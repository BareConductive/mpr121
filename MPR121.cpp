extern "C" {
	#include <stdlib.h>
	#include <string.h>
	#include <inttypes.h>
}

#include "MPR121.h"
#include "MPR121_defs.h"
#include <Wire.h>

MPR121_t::MPR121_t()
{
	address = 0x5A; // default address is 0x5A
}

void MPR121_t::begin(){

	reset();

	// default values...
	
	/******** NORMAL TOUCH ELECTRODES ********/
	
	// baseline filter rising 
	setRegister(MHDR,0x3F);
	setRegister(NHDR,0x3F);
	setRegister(NCLR,0x05);
	setRegister(FDLR,0x00);
	
	// baseline filter falling
	setRegister(MHDF,0x00);
	setRegister(NHDF,0x3F);
	setRegister(NCLF,0x10);
	setRegister(FDLF,0x03);
	
	// touched filter
	setRegister(NHDT,0x00);
	setRegister(NCLT,0x00);
	setRegister(FDLT,0xFF);
	
	/******** PROXIMITY SUPER-ELECTRODE ********/
	
	// baseline filter rising 
	setRegister(MHDPROXR,0x0F);
	setRegister(NHDPROXR,0x0F);
	setRegister(NCLPROXR,0x00);
	setRegister(FDLPROXR,0x00);

	// baseline filter falling
	setRegister(MHDPROXF,0x01);
	setRegister(NHDPROXF,0x01);
	setRegister(NCLPROXF,0xFF);
	setRegister(FDLPROXF,0xFF);

	// touched filter
	setRegister(NHDPROXT,0x00);
	setRegister(NCLPROXT,0x00);
	setRegister(FDLPROXT,0x00);
	
	// touch and release interrupt debounce
  	setRegister(DTR, 0x11);
  	
  	/******* EVERYTHING BELOW HERE IS
  	      	 POORLY UNDERSTOOD AND 
  	      	 UNOPTIMISED              **********/

	// AFE and filter configuration 
	setRegister(AFE1, 0xFF);  
	setRegister(AFE2, 0x38);
	setRegister(ECR, 0x0C);

	// Auto Configuration 
	setRegister(ACCR0, 0x0B);
	//setRegister(ACCR1, 0x80);
	setRegister(USL, 0xC8); 
	setRegister(LSL, 0x82); 
	setRegister(TL, 0xB4); 
	
	// Electrode Configuration
	//setRegister(ECR, 0xBC);
	setRegister(ECR, 0x80); // leaves all electrodes disabled
	
}

void MPR121_t::begin(unsigned char address){
	if(address>=0x5A && address<=0x5D) // addresses only valid 0x5A to 0x5D 
	{
		this->address = address; // need to be specific here
	}
	begin();
}

void MPR121_t::reset(){
	setRegister(SRST, 0x63); // soft reset
}

//private

void MPR121_t::setRegister(unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}

unsigned char MPR121_t::getRegister(unsigned char r){
    Wire.beginTransmission(address); 
    Wire.write(r); // set address register to read from our requested register
    Wire.endTransmission(false); // don't send stop so we can send a repeated start
    Wire.requestFrom(address,(unsigned char)1);  // just a single byte
    return Wire.read();
}

void MPR121_t::run(){
	setRegister(ECR, getRegister(ECR) | 0x3F); // turn on all electrodes to enter run
}

void MPR121_t::stop(){
	setRegister(ECR, getRegister(ECR) & 0xC0); // turn off all electrodes to enter stop
}

unsigned int MPR121_t::getTouchStatus(){
	return((unsigned int)getRegister(TS1) + ((unsigned int)getRegister(TS2)<<8));
}

bool MPR121_t::getTouchStatus(unsigned char electrode){
	if(electrode < 13){
		if(electrode<8){
			return((getRegister(TS1)>>electrode) & 0x01);
		} else {
			return((getRegister(TS2)>>(electrode-8)) & 0x01);;
		}
	} else return false;
}


void MPR121_t::setTouchThreshold(unsigned char val){
	for(unsigned char i=0; i<13; i++){
		setTouchThreshold(val, i);
	}
}

void MPR121_t::setTouchThreshold(unsigned char val, unsigned char electrode){
	touchThr[electrode] = val; 
	setRegister(E0TTH + (electrode<<1), val); 	// this relies on the internal register
												// map of the MPR121 and uses <<1 as
												// a quick equivalent to x2
}

void MPR121_t::setReleaseThreshold(unsigned char val){
	for(unsigned char i=0; i<13; i++){
		setReleaseThreshold(val,i);
	}
}

void MPR121_t::setReleaseThreshold(unsigned char val, unsigned char electrode){
	releaseThr[electrode] = val; 
	setRegister(E0RTH + (electrode<<1), val); 	// this relies on the internal register
												// map of the MPR121 and uses <<1 as
												// a quick equivalent to x2
}

MPR121_t MPR121 = MPR121_t();