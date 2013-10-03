extern "C" {
	#include <stdlib.h>
	#include <string.h>
	#include <inttypes.h>
}

#include "MPR121.h"
#include <Wire.h>

uint8_t MPR121_t::address = 0x5A; // default address is 0x5A

MPR121_t::MPR121_t()
{
}

MPR121_t::setAddress(uint8_t address){
	if(address>=0x5A && address<=0x5D) // addresses only valid 0x5A to 0x5D 
	{
		this->address = address;
	}
}



MPR121_t MPR121 = MPR121_t();