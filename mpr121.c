extern "C" {
	#include <stdlib.h>
	#include <string.h>
	#include <inttypes.h>
}

#include "MPR121.h"
#include "MPR121_defs"
#include <Wire.h>

uint8_t MPR121_t::address = 0x5A; // default address is 0x5A

MPR121_t::MPR121_t()
{
}

MPR121_t::begin(){
	
}

MPR121_t::setAddress(uint8_t address){
	if(address>=0x5A && address<=0x5D) // addresses only valid 0x5A to 0x5D 
	{
		this->address = address;
	}
}

MPR121_t::reset(){
	set_register(this->address, SRST, 0x63); // soft reset
}

//private

MPR121_t::set_register(unsigned char r, unsigned char v){
    Wire.beginTransmission(this->address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}

MPR121_t::get_register(unsigned char r){
    Wire.beginTransmission(this->address); 
    Wire.write(r); // set address register to read from our requested register
    Wire.endTransmission(false); // don't send stop so we can send a repeated start
    Wire.requestFrom(this->address,1);  // just a single byte
    return Wire.read();
    //Wire.endTransmission(true);
}

MPR121_t MPR121 = MPR121_t();