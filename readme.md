<a href="http://www.bareconductive.com/"><img src="https://www.dropbox.com/s/5c4yatkrnqqbgrm/BareConductive.LOGO_553x221.jpg?dl=1" width="200px" alt="Bare Conductive" /></a>

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
* callback attachment for interrupt pin

## Requirements

* requires Wire.h library, which is part of the Arduino core (so you don't need to download anything) but you do need to include it in your .ino file
* it is the user's responsibility to include <wire.h> and  call its begin() method
* before you use any of the MPR121 methods, you need to call its MPR121.begin() method, normally in setup()
* the default MPR121 I2C address for this library is 0x5C, which is used if you call MPR121.begin()
* if you want to specify a different address you can, with MPR121.begin(yourAddress)

Typical use example:

```
#define MPR121addr 0x5A

#include <Wire.h>
#include <MPR121.h>

void setup(){
	Wire.begin();
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

## Touch Board Quick Start Guide

The Touch Board is a easy-to-use platform for a huge range of interactive projects. Use the Touch Board to change the world around you by turning almost any material or surface into a sensor

## Getting Started with the Touch Board

The Touch Board comes pre-installed with a program and audio files on the micro SD card so it works out the box. Before we get ahead of our selves lets run through the following first.

1.	Open: Pop open the box and remove the Touch Board from its packaging
2.	Power: Give juice to the Touch Board! Use a LiPo battery or a micro USB cable.
3. 	ON: Switch the board on using the switch in the bottom left hand corner!
4. 	Audio: Plug in a pair of headphones or a powered speaker into the audio jack.
5.	Electrodes: Put the headphones on and listen to the audio guide by touching each electrode in turn. 
6.	Tracks: Change the audio tracks on the micro SD card by creating your own MP3 files on your computer and numbering them Track000.mp3 to Track011.mp3
7.	Change: Replace the tracks on the SD card from the audio guide with your newly create tracks
8. 	Click: Slot the micro SD card back into the Touch Board making sure you hear it click into place.
9. 	Reset: Hit the reset button to recalibrate the electrodes and reboot the program.
10.	Play: Have fun painting sensor to the Touch Board and triggering your audio tracks!

For more detailed information on getting up and running with your Touch Board check out the [Getting Starter with the Touch Board](http://www.bareconductive.com/make/introducing-the-touch-board/) tutorial.

## Setting up your Touch Board with Arduino 

1.	Go to www.arduino.cc and download the latest BETA version of the Arduino IDE
2.	Follow the instructions for installing Arduino on your operating system
3.	Download the Bare Conductive Hardware plugin from gitHub here and place in it the your hardware holder
4. 	Install the MPR121 and VS1053 libraries into your Libraries folder in Arduino.
5. 	Download Touch_MP3 and place it in your Arduino folder.
6.	Re-open Arduino and plug your Touch Board into your computer.
7.	Open the Touch_MP3 sketch in Arduino
8. 	Switch the board on and select ‘Bare Conductive Touch Board’ from the tools>>board drop down
9.	Select the correct port in tools>>port, choose the one with TTY in the name.
10.	Upload the code and your ready to roll!

For more detailed information on settng up Arduino for you Touch Board check out the [Setting up your Touch Board with Arduino](http://www.bareconductive.com/make/setting-up-arduino-with-your-touch-board/) tutorial.