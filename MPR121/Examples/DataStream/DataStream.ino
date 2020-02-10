/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------

 DataStream.ino - prints capacitive sense data from MPR121 to serial port

 Based on code by Jim Lindblom and plenty of inspiration from the Freescale
 Semiconductor datasheets and application notes.

 Bare Conductive code written by Stefan Dzisiewski-Smith, Peter Krige, Pascal
 Loose and Szymon Kaliski.

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

// touch includes
#include <MPR121.h>
#include <MPR121_Datastream.h>
#include <Wire.h>

// touch constants
const uint32_t BAUD_RATE = 115200;
const uint8_t MPR121_ADDR = 0x5C;
const uint8_t MPR121_INT = 4;

// MPR121 datastream behaviour constants
const bool MPR121_SAVED_THRESHOLDS = true;

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);

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

  MPR121.setInterruptPin(MPR121_INT);

  if (MPR121_SAVED_THRESHOLDS) {
    MPR121.restoreSavedThresholds();
  } else {
    MPR121.setTouchThreshold(40);
    MPR121.setReleaseThreshold(20);
  }

  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);  // reasonable for larger capacitances

  digitalWrite(LED_BUILTIN, HIGH);  // switch on user LED while auto calibrating electrodes
  delay(1000);
  MPR121.autoSetElectrodes();  // autoset all electrode settings
  digitalWrite(LED_BUILTIN, LOW);

  MPR121_Datastream.begin(&Serial);  // start datastream object using provided Serial reference
}

void loop() {
  MPR121.updateAll();
  MPR121_Datastream.update();
}
