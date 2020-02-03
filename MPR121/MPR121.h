/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------

 MPR121.h - MPR121 class header file

 Based on code by Jim Lindblom and plenty of inspiration from the Freescale
 Semiconductor datasheets and application notes.

 Bare Conductive code written by Stefan Dzisiewski-Smith, Peter Krige
 and Szymon Kaliski.

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

struct MPR121_settings_type
{
  // touch and release thresholds
  uint8_t TTHRESH;
  uint8_t RTHRESH;

  uint8_t INTERRUPT;

  // general electrode touch sense baseline filters
  // rising filter
  uint8_t MHDR;
  uint8_t NHDR;
  uint8_t NCLR;
  uint8_t FDLR;

  // falling filter
  uint8_t MHDF;
  uint8_t NHDF;
  uint8_t NCLF;
  uint8_t FDLF;

  // touched filter
  uint8_t NHDT;
  uint8_t NCLT;
  uint8_t FDLT;

  // proximity electrode touch sense baseline filters
  // rising filter
  uint8_t MHDPROXR;
  uint8_t NHDPROXR;
  uint8_t NCLPROXR;
  uint8_t FDLPROXR;

  // falling filter
  uint8_t MHDPROXF;
  uint8_t NHDPROXF;
  uint8_t NCLPROXF;
  uint8_t FDLPROXF;

  // touched filter
  uint8_t NHDPROXT;
  uint8_t NCLPROXT;
  uint8_t FDLPROXT;

  // debounce settings
  uint8_t DTR;

  // configuration registers
  uint8_t AFE1;
  uint8_t AFE2;
  uint8_t ECR;

  // auto-configuration registers
  uint8_t ACCR0;
  uint8_t ACCR1;
  uint8_t USL;
  uint8_t LSL;
  uint8_t TL;

  // default values in initialisation list
  MPR121_settings_type():
    TTHRESH(40),
    RTHRESH(20),
    INTERRUPT(4),   // note that this is not a hardware interrupt, just the digital
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
enum mpr121_pinf_type
{
  // INPUT and OUTPUT (and others) are already defined by Arduino, use its definitions if they exist
#ifndef INPUT
  INPUT,      // digital input
#endif
#ifndef INPUT_PULLUP
  INPUT_PULLUP,   // digital input with pullup
#endif
#ifndef INPUT_PULLDOWN
  INPUT_PULLDOWN,   // digital input with pulldown
#endif
#ifndef OUTPUT
  OUTPUT,     // digital output (push-pull)
#endif
#ifndef OUTPUT_HIGHSIDE
  OUTPUT_HIGHSIDE,  // digital output, open collector (high side)
#endif
#ifndef OUTPUT_LOWSIDE
  OUTPUT_LOWSIDE    // digital output, open collector (low side)
#endif
};

// "13th electrode" proximity modes
// N.B. this does not relate to normal proximity detection
// see http://cache.freescale.com/files/sensors/doc/app_note/AN3893.pdf
enum mpr121_proxmode_type
{
  PROX_DISABLED,    // proximity mode disabled
  PROX_0_1,     // proximity mode for ELE0..ELE1
  PROX_0_3,     // proximity mode for ELE0..ELE3
  PROX_0_11     // proximity mode for ELE0..ELE11
};

// baseline calibration lock modes
enum mpr121_cal_lock_type
{
  CAL_LOCK_ENABLED,       // baseline tracking enabled, no value change on run
  CAL_LOCK_DISABLED,        // baseline tracking disabled
  CAL_LOCK_ENABLED_5_BIT_COPY,  // baseline tracking enabled, load 5 MSB of filtered data on entering run mode
  CAL_LOCK_ENABLED_10_BIT_COPY  // baseline tracking enabled, load 10 MSB of filtered data on entering run mode
};

// error codes
enum mpr121_error_type
{
  NO_ERROR,     // no error
  RETURN_TO_SENDER, // not implemented
  ADDRESS_UNKNOWN,  // no MPR121 found at specified I2C address
  READBACK_FAIL,    // readback from MPR121 was not as expected
  OVERCURRENT_FLAG, // overcurrent on REXT pin
  OUT_OF_RANGE,   // autoconfiguration fail, often a result of shorted pins
  NOT_INITED      // device has not been initialised
};

// sample intervals
enum mpr121_sample_interval_type
{
  SAMPLE_INTERVAL_1MS   = 0x00,
  SAMPLE_INTERVAL_2MS   = 0x01,
  SAMPLE_INTERVAL_4MS   = 0x02,
  SAMPLE_INTERVAL_8MS   = 0x03,
  SAMPLE_INTERVAL_16MS  = 0x04,
  SAMPLE_INTERVAL_32MS  = 0x05,
  SAMPLE_INTERVAL_64MS  = 0x06,
  SAMPLE_INTERVAL_128MS = 0x07
};

// charge / discharge times (CDT)
enum mpr121_CDT_type
{
  CDT_500NS = 0x01,
  CDT_1US   = 0x02,
  CDT_2US   = 0x03,
  CDT_4US   = 0x04,
  CDT_8US   = 0x05,
  CDT_16US  = 0x06,
  CDT_32US  = 0x07
};

// first filter iterations (FFI)
enum mpr121_FFI_type
{
  FFI_6  = 0x00,
  FFI_10 = 0x01,
  FFI_18 = 0x02,
  FFI_34 = 0x03
};

// second filter iterations (SFI)
enum mpr121_SFI_type
{
  SFI_4  = 0x00,
  SFI_6  = 0x01,
  SFI_10 = 0x02,
  SFI_18 = 0x03
};

class MPR121_type
{
  public:
    MPR121_type();

    // -------------------- BASIC FUNCTIONS --------------------

    // begin() must be called before using any other function
    // address is optional, default is 0x5C
    bool begin(uint8_t address = 0x5C, uint8_t touchThreshold = 40, uint8_t releaseThreshold = 20, uint8_t interruptPin = 4);

    // read touch and release threshold saved to EEPROM using
    // saveTouchThreshold and releaseTouchThreshold
    void restoreSavedThresholds();

    // reset saved thresholds to 255, which will force the MPR121
    // to load default thresholds
    void clearSavedThresholds();

    // I2C speed control functions - goFast() sets the SCL clock
    // to 400kHz - goSlow() sets the SCL clock to 100kHz. Defaults
    // to 100kHz and affects all devices on the I2C bus. Included
    // for speed freaks only.
    void goSlow();
    void goFast();

    // getError() returns an mpr121_error_type indicating the current
    // error on the MPR121 - clearError() clears this
    mpr121_error_type getError();
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
    bool getTouchData(uint8_t electrode);

    // returns the number of touches currently detected
    uint8_t getNumTouches();

    // returns continous proximity or baseline data for a given electrode
    int getFilteredData(uint8_t electrode);
    int getBaselineData(uint8_t electrode);

    // returns boolean indicating whether a new touch or release has been
    // detected since the last time updateTouchData() was called
    bool isNewTouch(uint8_t electrode);
    bool isNewRelease(uint8_t electrode);

    // sets touch and release thresholds either for all electrodes, or
    // for a specfic electrode - higher values = less sensitive and
    // release threshold must ALWAYS be lower than touch threshold
    void setTouchThreshold(uint8_t val);
    void setTouchThreshold(uint8_t electrode, uint8_t val);
    void saveTouchThreshold(uint8_t electrode, uint8_t val);

    void setReleaseThreshold(uint8_t val);
    void setReleaseThreshold(uint8_t electrode, uint8_t val);
    void saveReleaseThreshold(uint8_t electrode, uint8_t val);

    // returns the current touch or release threshold for a specified electrode
    uint8_t getTouchThreshold(uint8_t electrode);
    uint8_t getReleaseThreshold(uint8_t electrode);

    // ------------------ ADVANCED FUNCTIONS ------------------

    // applies a complete array of settings from an
    // MPR121_settings_type variable passed as a pointer
    // useful if you want to do a bulk setup of the device
    void applySettings(MPR121_settings_type *settings);

    // setRegister() and getRegister() manipulate registers on
    // the MPR121 directly, whilst correctly stopping and
    // restarting the MPR121 if necessary
    void setRegister(uint8_t reg, uint8_t value);
    uint8_t getRegister(uint8_t reg);

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
    void setInterruptPin(uint8_t pin);

    // set number of electrodes to use to generate virtual "13th"
    // proximity electrode
    // see http://cache.freescale.com/files/sensors/doc/app_note/AN3893.pdf
    //
    // N.B. - this is not related to general proximity detection or
    // reading back continuous proximity data
    void setProxMode(mpr121_proxmode_type mode);

    // set calibration lock mode for baseline tracking
    // this can be useful to disable baseline tracking, set it to
    // current values (with 5 or 10 most significant bits copied across from
    // current filtered values to baseline the next time run mode is entered)
    // or leave it enabled with its current values in place (and no copy-across
    // when run mode is re-entered)
    void setCalibrationLock(mpr121_cal_lock_type lock);

    // set global or per-electrode charge / discharge current in microamps
    // current_uA is valid 0..63 (for global value, 0 means use electrode-specific setting)
    // electrode is valid 0..12 (includes virtual proximity electrode)
    void setGlobalCDC(uint8_t CDC);
    void setElectrodeCDC(uint8_t electrode, uint8_t CDC);

    // runs hardware routine for automatic electrode calibration for all electrodes
    // this is implemented in the MPR121 hardware itself
    // fixedChargeTime flag keeps CDT constant (useful for carbon-based inks with high lead-in resistance)
    bool autoSetElectrodes(uint16_t VCC_mV, bool fixedChargeTime = true);
    bool autoSetElectrodes(bool fixedChargeTime = true);

    // software routine to keep CDT constant and adjust CDC to fit
    bool autoSetElectrodeCDC(uint8_t electrode, uint16_t VCC_mV);
    bool autoSetElectrodeCDC(uint8_t electrode);
    bool autoSetElectrodeCDC(); // runs autocal for all electrodes

    // set global or per-electrode charge / discharge time
    // CDT follows a 0.5(2^(n-1))uS rule, so an enum type is provided for clarity
    void setGlobalCDT(mpr121_CDT_type CDT);
    void setElectrodeCDT(uint8_t electrode, mpr121_CDT_type CDT);

    // Set / get the number of enabled electrodes, from 0 (which implicitly enters
    // stop mode) up to 12. This allows for a reduction in power consumption
    // when using fewer electrodes and faster update rates. Implementation is
    // similar to setNumDigPins below, butwith a different intent.
    void setNumEnabledElectrodes(uint8_t numElectrodes);
    uint8_t getNumEnabledElectrodes();

    // Enables GPIO mode for up to 8 of the MPR121 electrodes
    // starts with electrode 11 - i.e. setNumDigPins(1) sets just
    // electrode 11 as GPIO, setNumDigPins(2) sets electrodes 11
    // & 10 as GPIO, and so on. Electrodes 0 to 3 cannot be used
    // as GPIO
    //
    // N.B. electrodes are 3.3V and WILL be damaged if driven by
    // a greater voltage
    void setNumDigPins(uint8_t numPins);

    // Sets pin mode for an electrode already set as GPIO by
    // setNumDigPins() - see section "GPIO pin function constants"
    // for details
    void pinMode(uint8_t electrode, mpr121_pinf_type mode);
    void pinMode(uint8_t electrode, int mode);

    // Similar to digitalWrite in Arduino for GPIO electrode
    void digitalWrite(uint8_t electrode, uint8_t val);

    // Toggles electrode set as GPIO output
    void digitalToggle(uint8_t electrode);

    // Reads electrode set as GPIO input
    bool digitalRead(uint8_t electrode);

    // Writes PWM value to electrode set as GPIO output - very limited
    // (4 bit, although input to function is 0..255 to match Arduino,
    // internally reduced to 4 bit) and broken on ELE9 and ELE10
    // see https://community.freescale.com/thread/305474
    void analogWrite(uint8_t electrode, uint8_t val);

    // Sets the sample period of the MPR121 - the time between capacitive
    // readings. Higher values consume less power, but are less responsive.
    void setSamplePeriod(mpr121_sample_interval_type period);

    void setFFI(mpr121_FFI_type FFI);

    void setSFI(mpr121_SFI_type SFI);

  // functions / variables internal to the MPR121_type class - you cannot access these externally

  private:
    // internal helper functions and variables
    // not exposed externally to the user
    uint8_t address;
    MPR121_settings_type defaultSettings;
    uint8_t ECR_backup; // so we can re-enable the correct number of electrodes
                  // when recovering from stop mode
    uint8_t error;
    bool running;
    uint8_t interruptPin;

    int16_t filteredData[13];
    int16_t baselineData[13];
    uint16_t touchData;
    uint16_t lastTouchData;
    bool getLastTouchData(uint8_t electrode);
    bool autoTouchStatusFlag; // we use this to catch touch / release events that happen
                                        // during other update calls

};

extern MPR121_type MPR121;

#endif // MPR121_H
