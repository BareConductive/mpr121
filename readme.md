[![Bare Conductive](http://bareconductive.com/assets/images/LOGO_256x106.png)](http://www.bareconductive.com/)

# Bare Conductive MPR121 Arduino Library

A fairly feature complete library for the Resurgent Semiconductor [MPR121QR2](http://www.nxp.com/files/sensors/doc/data_sheet/MPR121.pdf). This is a popular capacitive touch sense IC featuring:

* 12 electrodes/capacitance sensing inputs in which 8 are 
multifunctional for LED driving and GPIO
* Integrated independent autocalibration for each electrode input
* Autoconfiguration of charge current and charge time for each 
electrode input
* Separate touch and release trip thresholds for each electrode, 
providing hysteresis and electrode independence 
* I2C interface, with IRQ Interrupt output to advise electrode status 
changes

This library was originally developed to support the [Bare Conductive Touch Board](http://www.bareconductive.com/touch-board). However, it should work fine (with adaptations) for any Arduino connected to an MPR121.

Library includes support for:

* capacitive touch sensing
* proximity mode for virtual 13th electrode
* readback of filtered capacitance measurements
* full GPIO including PWM write

Still to be implemented:

* callback attachment for interrupt pin

## Requirements

* requires Wire.h library, which is part of the Arduino core (so you don't need to download anything) but you do need to include it in your .ino file
* it is the user's responsibility to include <wire.h> and  call its begin() method
* before you use any of the MPR121 methods, you need to call its MPR121.begin() method, normally in setup()
* the default MPR121 I2C address for this library is 0x5C, which is used if you call MPR121.begin()
* if you want to specify a different address you can, with MPR121.begin(yourAddress)

Typical use example:

```
// 0x5A for a lot of boards out there
// 0x5C for the Touch Board

#define MPR121addr 0x5A

#include <Wire.h>
#include <MPR121.h>

void setup(){
	MPR121.begin(MPR121addr);
}

void loop(){
	// your code goes here
}
```

## Install

* Arduino's instructions for installing libraries are available [here](http://arduino.cc/en/Guide/Libraries)


## Examples

* [SimpleTouch](./MPR121/Examples/SimpleTouch/) - simple capacitive touch sense sketch with output via serial
* [DataStream](./MPR121/Examples/DataStream/) - serial data stream of capacitance data from MPR121
* [GPIOinverter](./MPR121/Examples/GPIOinverter/) - MPR121 GPIO inverter - reads one pin and outputs the inverse on another
* [LEDfade](./MPR121/Examples/LEDfade/) - simple MPR121 LED fader

## Note

The new `clearSavedThresholds()`, `restoreSavedThresholds()` and `saveTouchThreshold()` methods for the MPR121 object use EEPROM to save and restore thresholds automatically when setting them with the updated Grapher Processing sketch. However, if you're using a board that doesn't have the AVR architecture,  your code will compile and run normally in every other way, but these functions will not work. Also, the new Datastream object's `reset()` method will do nothing due to the different way that watchdog timers are handled on AVR boards vs. other boards.