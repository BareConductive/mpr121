/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------

 MPR121_Datastream.h - MPR121_Datastream class header file

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

#ifndef MPR121_DATASTREAM_H
#define MPR121_DATASTREAM_H

#include <Arduino.h>
#include "MPR121.h"

class MPR121_Datastream_type {
public:
  MPR121_Datastream_type(){};
  ~MPR121_Datastream_type(){};

  void begin();
  void begin(int baudRate);
  void begin(Stream *stream);
  void begin(MPR121_type *mpr121);
  void begin(MPR121_type *mpr121, Stream *stream);
  void begin(MPR121_type *mpr121, int baudRate);

  void processSerial();
  void print();
  void update();

private:
  MPR121_type *mpr121; // mpr121 instance
  Stream *stream; // hardware or software serial port
};

extern MPR121_Datastream_type MPR121_Datastream;

#endif // MPR121_DATASTREAM_H
