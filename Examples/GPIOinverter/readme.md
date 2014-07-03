<a href="http://www.bareconductive.com/"><img src="https://www.dropbox.com/s/5c4yatkrnqqbgrm/BareConductive.LOGO_553x221.jpg?dl=1" width="200px" alt="Bare Conductive" /></a>

# Bare Conductive Touch Board GPIO Inverter Example

A GPIO inverter written using the MPR121 as a GPIO extender on the [Bare Conductive Touch Board](http://www.bareconductive.com/touch-board). Reads an MPR121 electrode and outputs the inverse to drive an LED connected to another electrode.

Needs an LED with a series current limiting resistor connected between E11 and ground. 470 ohms is good for most LEDs. E10 is set as a digital input with an internal pullup. Leaving E10 unconnected turns the LED off. Connecting E10 to ground turns the LED on.

##### TODO: diagram

## Requirements

* [Bare Conductive MPR121 library](../)
