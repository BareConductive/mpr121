<a href="http://www.bareconductive.com/"><img src="https://www.dropbox.com/s/5c4yatkrnqqbgrm/BareConductive.LOGO_553x221.jpg?dl=1" width="200px" alt="Bare Conductive" /></a>

# Bare Conductive Touch Board GPIO Inverter Example

A GPIO inverter written using the MPR121 as a GPIO extender on the [Bare Conductive Touch Board](http://www.bareconductive.com/touch-board). Reads an MPR121 electrode and outputs the inverse to drive an LED connected to another electrode.

Needs an LED with a series current limiting resistor connected between E11 and ground. 470 ohms is good for most LEDs. E10 is set as a digital input with an internal pullup. Leaving E10 unconnected turns the LED off. Connecting E10 to ground turns the LED on.

##### TODO: diagram

## Requirements

* [Arduino](http://arduino.cc/en/Main/Software) 1.0.5 or 1.5.6+ (1.5.6+ is strongly preferred).

* Arduino's instructions for installing libraries are available [here](http://arduino.cc/en/Guide/Libraries)

* [Bare Conductive MPR121 library](https://github.com/BareConductive/mpr121/)

## Install

This is normally installed as an example within the MPR121 library, but if you want to manually install it into your Arduino sketchbook, here is how to do it.

1. Close the Arduino IDE if you have it open.
1. Download the [.zip](https://github.com/BareConductive/mpr121/archive/public.zip) or clone the repository to your local machine - if downloading the .zip, extract the contents somewhere that suits you.
1. Take the **GPIOinverter** folder and move it to **Arduino Sketchbook Folder**. This will be different for each operating system: 

	**Windows**
	
	Libraries\\Documents\\Arduino
	
	or
	
	My Documents\\Arduino	
	
	**Mac**
	
	Documents/Arduino
	
	**Linux (Ubuntu)**
	
	Home/Arduino


	If this folder does not exist, create it first.
1. Reopen the Arduino IDE - you should now be able to open the sketch in the **File -> Sketchbook** menu.
