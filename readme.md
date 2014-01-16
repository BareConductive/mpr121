![Bare Conductive](http://www.bareconductive.com/content/themes/bareconductive/images/bare-conductive-logo.png)

# Bare Conductive MPR121 Arduino Library

A fairly feature complete library for the Freescale [MPR121QR2](http://www.freescale.com/files/sensors/doc/data_sheet/MPR121.pdf). This is a popular capacitive touch sense IC featuring:

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

* full support for autocalibration / autoconfig

## Install

* Arduino's instructions for installing libraries are available [here](http://arduino.cc/en/Guide/Libraries)

## Examples

* [SimpleTouch.ino](./Examples/SimpleTouch/SimpleTouch.ino) - simple capacitive touch sense sketch with output via serial
* [DataStream.ino](./Examples/DataStream/DataStream.ino) - serial data stream of capacitance data from MPR121
* [LEDfade.ino](./Examples/LEDfade/LEDfade.ino) - simple MPR121 LED fader