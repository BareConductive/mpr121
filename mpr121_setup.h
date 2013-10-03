// touch behaviour definitions
#define TouchThr 40 // 
#define ReleaseThr 20 

#define MPR121_ADDR 0x5A // address set on touch shield

#include <Wire.h>
#include "mpr121_defs.h"

void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}

unsigned char get_register(int address, unsigned char r){
    Wire.beginTransmission(address); 
    Wire.write(r); // set address register to read from our requested register
    Wire.endTransmission(false); // don't send stop so we can send a repeated start
    Wire.requestFrom(address,1);  // just a single byte
    return Wire.read();
    //Wire.endTransmission(true);
}

void mpr121_setup(void){
  
  // this code is all taken directly from the MPR121 Touch Sensing Design Guidelines document
  
  set_register(MPR121_ADDR, SRST, 0x63); // soft reset if MPR121 not reset properly
}

void set_touch_output(int pin, bool out){
  
  // sets a pin as an output if out = true
  // otherwise
  
  if(pin<0 || pin>7) return; // only deal with appropriate requests
  
  unsigned char scratch = get_register(MPR121_ADDR, DIR); // direction register
  if(out){
    scratch |= 1<<pin;  
  } else {
    scratch &= !(1<<pin);
  }
  set_register(MPR121_ADDR, DIR, scratch);
  
  scratch = get_register(MPR121_ADDR, EN); // enable register
  if(out){
    scratch |= 1<<pin;  
  } else {
    scratch &= !(1<<pin);
  }
  set_register(MPR121_ADDR, EN, scratch);
}

void write_touch_output(int pin, int value){
  if(pin<0 || pin>7) return; // only deal with appropriate requests
  
  if(value){
    set_register(MPR121_ADDR, SET, 1<<pin);
  } else {
    set_register(MPR121_ADDR, CLR, 1<<pin);
  }  
}

void write_touch_pwm(int pin, int value){
  // value valid 0..15
  if(pin<0 || pin>7) return; // only deal with appropriate requests
  
  if(value > 15) value = 15;
  if(value < 0) value = 0;
  
  unsigned char scratch;
  
  switch(pin){
  
    case 0:
      scratch = get_register(MPR121_ADDR, PWM0);
      set_register(MPR121_ADDR, PWM0, (value & 0x0F) | (scratch & 0xF0));
      break;
    case 1:
      scratch = get_register(MPR121_ADDR, PWM0);
      set_register(MPR121_ADDR, PWM0, ((value & 0x0F)<<4) | (scratch & 0x0F));
      break;
    case 2:
      scratch = get_register(MPR121_ADDR, PWM1);
      set_register(MPR121_ADDR, PWM1, (value & 0x0F) | (scratch & 0xF0));
      break;
    case 3:
      scratch = get_register(MPR121_ADDR, PWM1);
      set_register(MPR121_ADDR, PWM1, ((value & 0x0F)<<4) | (scratch & 0x0F));
      break;
    case 4:
      scratch = get_register(MPR121_ADDR, PWM2);
      set_register(MPR121_ADDR, PWM2, (value & 0x0F) | (scratch & 0xF0));
      break;
    case 5:
      scratch = get_register(MPR121_ADDR, PWM2);
      set_register(MPR121_ADDR, PWM2, ((value & 0x0F)<<4) | (scratch & 0x0F));
      break;
    case 6:
      scratch = get_register(MPR121_ADDR, PWM3);
      set_register(MPR121_ADDR, PWM3, (value & 0x0F) | (scratch & 0xF0));
      break;
    case 7:
      scratch = get_register(MPR121_ADDR, PWM3);
      set_register(MPR121_ADDR, PWM3, ((value & 0x0F)<<4) | (scratch & 0x0F));
      break;
  }

  
}
