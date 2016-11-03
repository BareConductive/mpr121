/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------
 
 MPR121.h - MPR121 class header file
 
 Based on code by Jim Lindblom and plenty of inspiration from the Freescale 
 Semiconductor datasheets and application notes.
 
 Bare Conductive code written by Stefan Dzisiewski-Smith and Peter Krige.
 
 This work is licensed under a MIT license https://opensource.org/licenses/MIT
 
 Copyright (c) 2016, Bare Conductive
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

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
		MHDR(0x01),
		NHDR(0x01),
		NCLR(0x10),
		FDLR(0x20),
		MHDF(0x01),
		NHDF(0x01),
		NCLF(0x10),
		FDLF(0x20),
		NHDT(0x01),
		NCLT(0x10),
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
		AFE2(0x30),
		ECR(0xCC), // default to fast baseline startup and 12 electrodes enabled, no prox
		ACCR0(0x00),
		ACCR1(0x00),
		USL(0x00), 
		LSL(0x00), 
		TL(0x00) {}
	
};

// GPIO pin function constants
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

// "13th electrode" proximity modes
// N.B. this does not relate to normal proximity detection
// see http://cache.freescale.com/files/sensors/doc/app_note/AN3893.pdf
enum mpr121_proxmode_t
{
	DISABLED,		// proximity mode disabled
	PROX0_1,		// proximity mode for ELE0..ELE1
	PROX0_3,		// proximity mode for ELE0..ELE3
	PROX0_11		// proximity mode for ELE0..ELE11		
};

// error codes
enum mpr121_error_t
{
	NO_ERROR,			// no error
	RETURN_TO_SENDER,	// not implemented
	ADDRESS_UNKNOWN,	// no MPR121 found at specified I2C address
	READBACK_FAIL,		// readback from MPR121 was not as expected
	OVERCURRENT_FLAG,	// overcurrent on REXT pin
	OUT_OF_RANGE,		// autoconfiguration fail, often a result of shorted pins
	NOT_INITED			// device has not been initialised
};

// sample intervals
enum mpr121_sample_interval_t
{
	SAMPLE_INTERVAL_1MS = 0x00,
	SAMPLE_INTERVAL_2MS = 0x01,
	SAMPLE_INTERVAL_4MS = 0x02,
	SAMPLE_INTERVAL_8MS = 0x03,
	SAMPLE_INTERVAL_16MS = 0x04,
	SAMPLE_INTERVAL_32MS = 0x05,
	SAMPLE_INTERVAL_64MS = 0x06,
	SAMPLE_INTERVAL_128MS = 0x07
};

class MPR121_t
{		
	public:
		MPR121_t();

		// -------------------- BASIC FUNCTIONS --------------------

		// begin() must be called before using any other function
		// address is optional, default is 0x5C
		bool begin(unsigned char address);
		bool begin();

		// I2C speed control functions - goFast() sets the SCL clock
		// to 400kHz - goSlow() sets the SCL clock to 100kHz. Defaults
		// to 100kHz and affects all devices on the I2C bus. Included
		// for speed freaks only.
		void goSlow();
		void goFast();

		// getError() returns an mpr121_error_t indicating the current 
		// error on the MPR121 - clearError() clears this
		mpr121_error_t getError();
		void clearError();

		// returns status of the MPR121 INT pin as read via digitalRead() on the
		// Arduino board - this tells us if there has been a change in touch status
		// on any active electrode since we last read any data
		bool touchStatusChanged();		
		
		// updates the data from the MPR121 into our internal buffer
		// updateTouchData() does this only for touch on / off status
		// updateBaseLineData() does this for background baseline
		// updateFilteredData() does this for continuous proximity data
		// updateAll() does all three

		// the appropriate function from these must be called before data
		// from getTouchData(), getFilteredData() etc. can be considered
		// valid
		void updateTouchData();
		bool updateBaselineData();
		bool updateFilteredData();
		void updateAll();
		
		// returns a boolean indicating the touch status of a given electrode
		bool getTouchData(unsigned char electrode);	

		// returns the number of touches currently detected
		unsigned char getNumTouches();

		// returns continous proximity or baseline data for a given electrode
		int getFilteredData(unsigned char electrode);
		int getBaselineData(unsigned char electrode);

		// returns boolean indicating whether a new touch or release has been
		// detected since the last time updateTouchData() was called
		bool isNewTouch(unsigned char electrode);
		bool isNewRelease(unsigned char electrode);		
		
		// sets touch and release thresholds either for all electrodes, or 
		// for a specfic electrode - higher values = less sensitive and 
		// release threshold must ALWAYS be lower than touch threshold
		void setTouchThreshold(unsigned char val);
		void setTouchThreshold(unsigned char electrode, unsigned char val);
		void setReleaseThreshold(unsigned char val);
		void setReleaseThreshold(unsigned char electrode, unsigned char val);

		// returns the current touch or release threshold for a specified electrode
		unsigned char getTouchThreshold(unsigned char electrode);
		unsigned char getReleaseThreshold(unsigned char electrode);	
		
		// ------------------ ADVANCED FUNCTIONS ------------------

		// applies a complete array of settings from an
		// MPR121_settings_t variable passed as a pointer
		// useful if you want to do a bulk setup of the device
		void applySettings(MPR121_settings_t *settings);

		// setRegister() and getRegister() manipulate registers on
		// the MPR121 directly, whilst correctly stopping and
		// restarting the MPR121 if necessary
		void setRegister(unsigned char reg, unsigned char value);
		unsigned char getRegister(unsigned char reg);

		// stop() and run() take the MPR121 in and out of stop mode
		// which reduces current consumption to 3uA
		void run();
		void stop();

		// resets the MPR121
		bool reset();

		// tells us if we are in run mode, and if we have inited the
		// MPR121
		bool isRunning();
		bool isInited();		

		// sets the pin that the MPR121 INT output is connected to on the 
		// Arduino board - does not have to be a hardware interrupt pin
		// if it is, however, an interrupt service routine will automatically
		// set an internal flag when a touch event occurs - thus minimising
		// lost events if you are also reading other data types (filtered data,
		// baseline data)
		void setInterruptPin(unsigned char pin);

		// set number of electrodes to use to generate virtual "13th"
		// proximity electrode 
		// see http://cache.freescale.com/files/sensors/doc/app_note/AN3893.pdf
		//
		// N.B. - this is not related to general proximity detection or
		// reading back continuous proximity data
		void setProxMode(mpr121_proxmode_t mode);	

		// Enables GPIO mode for up to 8 of the MPR121 electrodes
		// starts with electrode 11 - i.e. setNumDigPins(1) sets just
		// electrode 11 as GPIO, setNumDigPins(2) sets electrodes 11
		// & 10 as GPIO, and so on. Electrodes 0 to 3 cannot be used
		// as GPIO
		//
		// N.B. electrodes are 3.3V and WILL be damaged if driven by
		// a greater voltage
		void setNumDigPins(unsigned char numPins);

		// Sets pin mode for an electrode already set as GPIO by
		// setNumDigPins() - see section "GPIO pin function constants"
		// for details
		void pinMode(unsigned char electrode, mpr121_pinf_t mode); 
		void pinMode(unsigned char electrode, int mode); 				

		// Similar to digitalWrite in Arduino for GPIO electrode
		void digitalWrite(unsigned char electrode, unsigned char val);

		// Toggles electrode set as GPIO output
		void digitalToggle(unsigned char electrode);

		// Reads electrode set as GPIO input
		bool digitalRead(unsigned char electrode);

		// Writes PWM value to electrode set as GPIO output - very limited
		// (4 bit, although input to function is 0..255 to match Arduino,
		// internally reduced to 4 bit) and broken on ELE9 and ELE10
		// see https://community.freescale.com/thread/305474
		void analogWrite(unsigned char electrode, unsigned char val);

		// Sets the sample period of the MPR121 - the time between capacitive
		// readings. Higher values consume less power, but are less responsive.
		void setSamplePeriod(mpr121_sample_interval_t period);

	// functions / variables internal to the MPR121_t class - you cannot access these externally

	private:
		// internal helper functions and variables
		// not exposed externally to the user
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
		bool autoTouchStatusFlag;	// we use this to catch touch / release events that happen
																				// during other update calls	

};

extern MPR121_t MPR121;

#endif // MPR121_H