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
	applySettings(&defaultSettings);
	
}

void MPR121_t::begin(unsigned char address){
	if(address>=0x5A && address<=0x5D) // addresses only valid 0x5A to 0x5D 
	{
		this->address = address; // need to be specific here
	}
	begin();
}

void MPR121_t::reset(){
	setRegister(_SRST, 0x63); // soft reset
}

void MPR121_t::applySettings(MPR121_settings *settings){
	setTouchThreshold(settings->TTHRESH);
	setReleaseThreshold(settings->RTHRESH);
	setRegister(_MHDR,settings->MHDR);
	setRegister(_NHDR,settings->NHDR);
	setRegister(_NCLR,settings->NCLR);
	setRegister(_FDLR,settings->FDLR);
	setRegister(_MHDF,settings->MHDF);
	setRegister(_NHDF,settings->NHDF);
	setRegister(_NCLF,settings->NCLF);
	setRegister(_FDLF,settings->FDLF);
	setRegister(_NHDT,settings->NHDT);
	setRegister(_NCLT,settings->NCLT);
	setRegister(_FDLT,settings->FDLT);
	setRegister(_MHDPROXR,settings->MHDPROXR);
	setRegister(_NHDPROXR,settings->NHDPROXR);
	setRegister(_NCLPROXR,settings->NCLPROXR);
	setRegister(_FDLPROXR,settings->FDLPROXR);
	setRegister(_MHDPROXF,settings->MHDPROXF);
	setRegister(_NHDPROXF,settings->NHDPROXF);
	setRegister(_NCLPROXF,settings->NCLPROXF);
	setRegister(_FDLPROXF,settings->FDLPROXF);
	setRegister(_NHDPROXT,settings->NHDPROXT);
	setRegister(_NCLPROXT,settings->NCLPROXT);
	setRegister(_FDLPROXT,settings->FDLPROXT);
  	setRegister(_DTR, settings->DTR);
	setRegister(_AFE1, settings->AFE1);  
	setRegister(_AFE2, settings->AFE2);
	setRegister(_ECR, settings->ECR);
	setRegister(_ACCR0, settings->ACCR0);
	setRegister(_ACCR1, settings->ACCR1);
	setRegister(_USL, settings->USL); 
	setRegister(_LSL, settings->LSL); 
	setRegister(_TL, settings->TL); 
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
	setRegister(_ECR, getRegister(_ECR) | 0x3F); // turn on all electrodes to enter run
}

void MPR121_t::stop(){
	setRegister(_ECR, getRegister(_ECR) & 0xC0); // turn off all electrodes to enter stop
}

unsigned int MPR121_t::getTouchStatus(){
	return((unsigned int)getRegister(_TS1) + ((unsigned int)getRegister(_TS2)<<8));
}

bool MPR121_t::getTouchStatus(unsigned char electrode){
	if(electrode < 13){
		if(electrode<8){
			return((getRegister(_TS1)>>electrode) & 0x01);
		} else {
			return((getRegister(_TS2)>>(electrode-8)) & 0x01);;
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
	setRegister(_E0TTH + (electrode<<1), val); 	// this relies on the internal register
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
	setRegister(_E0RTH + (electrode<<1), val); 	// this relies on the internal register
												// map of the MPR121 and uses <<1 as
												// a quick equivalent to x2
}

MPR121_t MPR121 = MPR121_t();