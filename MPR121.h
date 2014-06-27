/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------
 
 MPR121.h - MPR121 class header file
 
 Based on code by Jim Lindblom and plenty of inspiration from the Freescale 
 Semiconductor datasheets and application notes.
 
 Bare Conductive code written by Stefan Dzisiewski-Smith and Peter Krige.
 
 This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 
 Unported License (CC BY-SA 3.0) http://creativecommons.org/licenses/by-sa/3.0/
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*******************************************************************************/

#ifndef MPR121_H
#define MPR121_H

#include "MPR121_defs.h"
#include <Wire.h>

// idea behind this is to create a settings structure that we can use to store 
// all the setup variables for a particular setup - comes pre-instantiated with 
// defaults and can be easily tweaked - we pass by reference (as a pointer) to 
// save RAM

struct MPR121_settings_t
{
	// touch and release thresholds
	unsigned char TTHRESH;
	unsigned char RTHRESH;
	
	unsigned char INTERRUPT;

	// general electrode touch sense baseline filters
	// rising filter
	unsigned char MHDR;
	unsigned char NHDR;
	unsigned char NCLR;
	unsigned char FDLR;

	// falling filter
	unsigned char MHDF;
	unsigned char NHDF;
	unsigned char NCLF;
	unsigned char FDLF;

	// touched filter
	unsigned char NHDT;
	unsigned char NCLT;
	unsigned char FDLT;

	// proximity electrode touch sense baseline filters
	// rising filter
	unsigned char MHDPROXR;
	unsigned char NHDPROXR;
	unsigned char NCLPROXR;
	unsigned char FDLPROXR;

	// falling filter
	unsigned char MHDPROXF;
	unsigned char NHDPROXF;
	unsigned char NCLPROXF;
	unsigned char FDLPROXF;

	// touched filter
	unsigned char NHDPROXT;
	unsigned char NCLPROXT;
	unsigned char FDLPROXT;

	// debounce settings
	unsigned char DTR;

	// configuration registers
	unsigned char AFE1;
	unsigned char AFE2;
	unsigned char ECR;

	// auto-configuration registers
	unsigned char ACCR0;
	unsigned char ACCR1;
	unsigned char USL; 
	unsigned char LSL; 
	unsigned char TL; 
	
	// default values in initialisation list
	MPR121_settings_t():
		TTHRESH(40),
		RTHRESH(20),
		INTERRUPT(4), 	// note that this is not a hardware interrupt, just the digital
						// pin that the MPR121 ~INT pin is connected to
		MHDR(0x3F),
		NHDR(0x3F),
		NCLR(0x05),
		FDLR(0x00),
		MHDF(0x00),
		NHDF(0x3F),
		NCLF(0x10),
		FDLF(0x03),
		NHDT(0x00),
		NCLT(0x00),
		FDLT(0xFF),
		MHDPROXR(0x0F),
		NHDPROXR(0x0F),
		NCLPROXR(0x00),
		FDLPROXR(0x00),
		MHDPROXF(0x01),
		NHDPROXF(0x01),
		NCLPROXF(0xFF),
		FDLPROXF(0xFF),
		NHDPROXT(0x00),
		NCLPROXT(0x00),
		FDLPROXT(0x00),
		DTR(0x11),
		AFE1(0xFF),
		AFE2(0x38),
		ECR(0x8C), // default to fast baseline startup and 12 electrodes enabled, no prox
		ACCR0(0x00),
		ACCR1(0x00),
		USL(0x00), 
		LSL(0x00), 
		TL(0x00) {}
	
};

enum mpr121_pinf_t
{
	// INPUT and OUTPUT are already defined by Arduino, use its definitions
	
	//INPUT,		// digital input
	INPUT_PU,		// digital input with pullup
	INPUT_PD,		// digital input with pulldown
	//OUTPUT,		// digital output (push-pull)
	OUTPUT_HS,		// digital output, open collector (high side)
	OUTPUT_LS		// digital output, open collector (low side)
};

enum mpr121_proxmode_t
{
	DISABLED,		// proximity mode disabled
	PROX0_1,		// proximity mode for ELE0..ELE1
	PROX0_3,		// proximity mode for ELE0..ELE3
	PROX0_11		// proximity mode for ELE0..ELE11		
};

enum mpr121_error_t
{
	NO_ERROR,
	RETURN_TO_SENDER, // not implemented
	ADDRESS_UNKNOWN,  
	READBACK_FAIL,
	OVERCURRENT_FLAG,
	OUT_OF_RANGE,
	NOT_INITED
};

class MPR121_t
{
	private:
		unsigned char address;
		MPR121_settings_t defaultSettings;
		unsigned char ECR_backup; // so we can re-enable the correct number of electrodes
								  // when recovering from stop mode
		unsigned char error;
		bool running;
		int interruptPin;
		
		int filteredData[13];
		int baselineData[13];
		unsigned int touchData;		  
		unsigned int lastTouchData;	  
		bool getLastTouchData(unsigned char electrode);			
		
	public:
		MPR121_t();

		void setRegister(unsigned char reg, unsigned char value);
		unsigned char getRegister(unsigned char reg);		
		bool begin(unsigned char address);
		bool begin();
		void run();
		void stop();
		bool reset();
		void applySettings(MPR121_settings_t *settings);
		
		mpr121_error_t getError();
		void clearError();
		
		bool isRunning();
		bool isInited();
		
		void updateTouchData();
		bool updateBaselineData();
		bool updateFilteredData();
		void updateAll();
		
		bool getTouchData(unsigned char electrode);	
		unsigned char getNumTouches();
		int getFilteredData(unsigned char electrode);
		int getBaselineData(unsigned char electrode);
		bool isNewTouch(unsigned char electrode);
		bool isNewRelease(unsigned char electrode);		
		
		void setTouchThreshold(unsigned char val);
		void setTouchThreshold(unsigned char electrode, unsigned char val);
		void setReleaseThreshold(unsigned char val);
		void setReleaseThreshold(unsigned char electrode, unsigned char val);
		unsigned char getTouchThreshold(unsigned char electrode);
		unsigned char getReleaseThreshold(unsigned char electrode);	
		
		void setInterruptPin(unsigned char pin);
		bool touchStatusChanged();	

		void setProxMode(mpr121_proxmode_t mode);		
		void setNumDigPins(unsigned char numPins);
		void pinMode(unsigned char electrode, mpr121_pinf_t mode); 
		void pinMode(unsigned char electrode, int mode); 				
		void digitalWrite(unsigned char electrode, unsigned char val);
		void digitalToggle(unsigned char electrode);
		bool digitalRead(unsigned char electrode);
		void analogWrite(unsigned char electrode, unsigned char val);
		
};

extern MPR121_t MPR121;

#endif // MPR121_H