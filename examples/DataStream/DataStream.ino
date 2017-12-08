/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------
 
 DataStream.ino - prints capacitive sense data from MPR121 to serial port
 
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

// serial rate
#define baudRate 57600

#include <MPR121.h>
#include <Wire.h>

// this is the touch threshold - setting it low makes it more like a proximity trigger
// default value is 40 for touch
const int touchThreshold = 40;
// this is the release threshold - must ALWAYS be smaller than the touch threshold
// default value is 20 for touch
const int releaseThreshold = 20;

void setup(){  
  Serial.begin(baudRate);
  while(!Serial); // only needed for Arduino Leonardo or Bare Touch Board 

  // 0x5C is the MPR121 I2C address on the Bare Touch Board
  if(!MPR121.begin(0x5C)){ 
    Serial.println("error setting up MPR121");  
    switch(MPR121.getError()){
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
    while(1);
  }


  MPR121.setTouchThreshold(touchThreshold);
  MPR121.setReleaseThreshold(releaseThreshold);  
}

void loop(){
   readRawInputs();  
}

void readRawInputs(){
    int i;
    
    if(MPR121.touchStatusChanged()) MPR121.updateTouchData();
    MPR121.updateBaselineData();
    MPR121.updateFilteredData();
    
    
    Serial.print("TOUCH: ");
    for(i=0; i<13; i++){          // 13 touch values
      Serial.print(MPR121.getTouchData(i), DEC);
      if(i<12) Serial.print(" ");
    }    
    Serial.println();   
    
    Serial.print("TTHS: ");
    for(i=0; i<13; i++){          // 13 touch thresholds
      Serial.print(touchThreshold, DEC); 
      if(i<12) Serial.print(" ");
    }   
    Serial.println();
    
    Serial.print("RTHS: ");
    for(i=0; i<13; i++){          // 13 release thresholds
      Serial.print(releaseThreshold, DEC); 
      if(i<12) Serial.print(" ");
    }   
    Serial.println();
    
    Serial.print("FDAT: ");
    for(i=0; i<13; i++){          // 13 filtered values
      Serial.print(MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    } 
    Serial.println();
    
    Serial.print("BVAL: ");
    for(i=0; i<13; i++){          // 13 baseline values
      Serial.print(MPR121.getBaselineData(i), DEC);
      if(i<12) Serial.print(" ");
    } 
    Serial.println();
    
    // the trigger and threshold values refer to the difference between
    // the filtered data and the running baseline - see p13 of 
    // http://www.freescale.com/files/sensors/doc/data_sheet/MPR121.pdf
    
    Serial.print("DIFF: ");
    for(i=0; i<13; i++){          // 13 value pairs
      Serial.print(MPR121.getBaselineData(i)-MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    }           
    Serial.println();

}
