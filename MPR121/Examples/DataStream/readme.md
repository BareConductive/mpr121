[![Bare Conductive](http://bareconductive.com/assets/images/LOGO_256x106.png)](http://www.bareconductive.com/)

# Bare Conductive Touch Board Datastream Example

Continuously reads capacitive sense data from the MPR121 on the [Bare Conductive Touch Board](http://www.bareconductive.com/touch-board) and outputs it to the (virtual) serial port. Can optionally interface with Bare Conductive's [MPR121 grapher](https://github.com/BareConductive/mpr121-grapher) or [Max/MSP communication patch](https://github.com/BareConductive/Max-MSP-Touch-Board-communication).

## Requirements

* You should make sure that you have followed our [Setting up Arduino with your Touch Board](http://www.bareconductive.com/make/setting-up-arduino-with-your-touch-board/) tutorial before using this (or any other) of our code examples

## Install

This is normally installed as an example within the MPR121 library, but if you want to manually install it into your Arduino sketchbook, here is how to do it.

1. Close the Arduino IDE if you have it open.
1. Download the [.zip](https://github.com/BareConductive/mpr121/archive/public.zip) or clone the repository to your local machine - if downloading the .zip, extract the contents somewhere that suits you.
1. Take the **DataStream** folder and move it to **Arduino Sketchbook Folder**. This will be different for each operating system: 

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

## Output message formatting

    TOUCH:		electrode touch values (0 not touched, 1 touched)
    TTHS:		electrode touch thresholds (0..255)
    RTHS:		electrode release thresholds (0..255)
    FDAT:		electrode filtered data (0..1023)
    BVAL:		electrode baseline values (0..1023)
    DIFF:		/bval - /fdat (0..1023)
 


