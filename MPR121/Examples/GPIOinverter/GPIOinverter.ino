/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------

 GPIOinverter.ino - MPR121 GPIO inverter - reads a pin, outputs the inverse

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

// Requires an LED with a series current limiting resistor connected between
// E11 and ground. 470 ohms is good for most LEDs. E10 is set as a digital input
// with an internal pullup. Leaving E10 unconnected turns the LED off.
// Connecting E10 to ground turns the LED on.

// touch includes
#include <MPR121.h>
#include <Wire.h>

// touch constants
const uint32_t BAUD_RATE = 115200;
const uint8_t MPR121_ADDR = 0x5C;

void setup() {
  Serial.begin(BAUD_RATE);

  if (!MPR121.begin(MPR121_ADDR)) {
    Serial.println("error setting up MPR121");
    switch (MPR121.getError()) {
      case NO_ERROR:
        Serial.println("no error");
        break;
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;
    }
    while (1);
  }

  // The MPR121 allows a mixture of GPIO and touch sense electrodes to be
  // selected for the 12 pins labelled E0..E11, but you can't just pick and
  // choose arbitrarily. The first four electrodes (E0..E3) are always touch
  // sense pins - they can't be anything else. Then you can set the number of
  // GPIO pins from 0 to 8 for the remaining pins. These are set sequentially
  // i.e. if 1 pin is required, this is ALWAYS E11, if 2 pins, E11 and E10
  // and so on up to 8 pins (E11..E4).

  // See p.20 of http://www.nxp.com/docs/en/data-sheet/MPR121.pdf
  // for more details.

  MPR121.setNumDigPins(2);

  // Note that each electrode has 7 possible pin modes (6 GPIO and 1 touch)
  // these are INPUT, INPUT_PULLUP (input with internal pullup), INPUT_PULLDOWN
  // (input with internal pulldown), OUTPUT, OUTPUT_HIGHSIDE (open collector
  // output, high-side), OUTPUT_LOWSIDE (open collector output, low side).

  // See p.3 of http://cache.freescale.com/files/sensors/doc/app_note/AN3894.pdf
  // for more details

  MPR121.pinMode(11, OUTPUT);
  MPR121.pinMode(10, INPUT_PULLUP);
}

void loop() {
  bool readVal = MPR121.digitalRead(10);  // read E10
  MPR121.digitalWrite(11, !readVal);  // write the inverse to E11
}
