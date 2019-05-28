/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------

 MPR121_Datastream_type.cpp - MPR121_Datastream_type class implementation file

 Based on code by Jim Lindblom and plenty of inspiration from the Freescale
 Semiconductor datasheets and application notes.

 Bare Conductive code written by Szymon Kaliski.

 This work is licensed under a MIT license https://opensource.org/licenses/MIT

 Copyright (c) 2019, Bare Conductive

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

#include "MPR121_Datastream.h"

#ifdef ARDUINO_ARCH_AVR
  #include <avr/wdt.h>
#endif

#define NUM_ELECTRODES 13

void reset() {
  // enable watchdog and force an infinite loop to reset
  // TODO: fix this for SAMD
  #ifdef ARDUINO_ARCH_AVR
    wdt_disable();
    wdt_enable(WDTO_15MS);

  while (true) {
  }

  #endif
}

char *splitString(char *data, char *separator, int index) {
  char *act, *sub, *ptr;
  static char copy[255];
  int i;

  strcpy(copy, data);

  for (i = 0, act = copy; i <= index; i++, act = NULL) {
    sub = strtok_r(act, separator, &ptr);

    if (sub == NULL) {
      break;
    }
  }

  return sub;
}

void MPR121_Datastream_type::begin() {
  Serial.begin(57600);
  stream = &Serial;
  mpr121 = &MPR121;
}

void MPR121_Datastream_type::begin(int baudRate) {
  Serial.begin(baudRate);
  stream = &Serial;
  mpr121 = &MPR121;
}

void MPR121_Datastream_type::begin(Stream *s) {
  stream = s;
  mpr121 = &MPR121;
}

void MPR121_Datastream_type::begin(MPR121_type *m) {
  Serial.begin(57600);
  stream = &Serial;
  mpr121 = m;
}

void MPR121_Datastream_type::begin(MPR121_type *m, int baudRate) {
  Serial.begin(baudRate);
  stream = &Serial;
  mpr121 = m;
}

void MPR121_Datastream_type::begin(MPR121_type *m, Stream *s) {
  stream = s;
  mpr121 = m;
}

void MPR121_Datastream_type::processSerial() {
  static byte ndx = 0;
  boolean hasData = false;

  char receivedChars[255];
  char receivedChar;

  while (stream->available() > 0 && !hasData) {
    receivedChar = stream->read();

    if (receivedChar != '\n') {
      receivedChars[ndx] = receivedChar;
      ndx++;

      if (ndx >= 255) {
        ndx = 254;
      }
    } else {
      receivedChars[ndx] = '\0';
      ndx = 0;
      hasData = true;
    }
  }

  if (hasData) {
    char *command = splitString(receivedChars, ":", 0);
    int electrodeNum = atoi(splitString(receivedChars, ":", 1));
    int value = atoi(splitString(receivedChars, ":", 2));

    if (strcmp(command, "STTH") == 0) {
      // set touch threshold
      mpr121->saveTouchThreshold(electrodeNum, value);
    }

    if (strcmp(command, "SRTH") == 0) {
      // set release threshold
      mpr121->saveReleaseThreshold(electrodeNum, value);
    }

    if (strcmp(command, "CLEAR") == 0) {
      mpr121->clearSavedThresholds();
      mpr121->restoreSavedThresholds();
    }

    if (strcmp(command, "RESET") == 0) {
      reset();
    }
  }
}

void MPR121_Datastream_type::print() {
  int i;

  stream->print("TOUCH: ");
  for (i = 0; i < NUM_ELECTRODES; i++) {
    stream->print(mpr121->getTouchData(i), DEC);
    if (i < NUM_ELECTRODES - 1) {
      stream->print(" ");
    }
  }
  stream->println();

  stream->print("TTHS: ");
  for (i = 0; i < NUM_ELECTRODES; i++) {
    stream->print(mpr121->getTouchThreshold(i), DEC);
    if (i < NUM_ELECTRODES - 1) {
      stream->print(" ");
    }
  }
  stream->println();

  stream->print("RTHS: ");
  for (i = 0; i < NUM_ELECTRODES; i++) {
    stream->print(mpr121->getReleaseThreshold(i), DEC);
    if (i < NUM_ELECTRODES - 1) {
      stream->print(" ");
    }
  }
  stream->println();

  stream->print("FDAT: ");
  for (i = 0; i < NUM_ELECTRODES; i++) {
    stream->print(mpr121->getFilteredData(i), DEC);
    if (i < NUM_ELECTRODES - 1) {
      stream->print(" ");
    }
  }
  stream->println();

  stream->print("BVAL: ");
  for (i = 0; i < NUM_ELECTRODES; i++) {
    stream->print(mpr121->getBaselineData(i), DEC);
    if (i < NUM_ELECTRODES - 1) {
      stream->print(" ");
    }
  }
  stream->println();

  stream->print("DIFF: ");
  for (i = 0; i < NUM_ELECTRODES; i++) {
    stream->print(mpr121->getBaselineData(i) - mpr121->getFilteredData(i), DEC);
    if (i < NUM_ELECTRODES - 1) {
      stream->print(" ");
    }
  }
  stream->println();
}

void MPR121_Datastream_type::update() {
  processSerial();
  print();
}

MPR121_Datastream_type MPR121_Datastream = MPR121_Datastream_type();
