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
	setRegister(SRST, 0x63); // soft reset
}

void MPR121_t::applySettings(MPR121_settings *settings){
	setTouchThreshold(settings->TTHRESH);
	setReleaseThreshold(settings->RTHRESH);
	setRegister(MHDR,settings->MHDR);
	setRegister(NHDR,settings->NHDR);
	setRegister(NCLR,settings->NCLR);
	setRegister(FDLR,settings->FDLR);
	setRegister(MHDF,settings->MHDF);
	setRegister(NHDF,settings->NHDF);
	setRegister(NCLF,settings->NCLF);
	setRegister(FDLF,settings->FDLF);
	setRegister(NHDT,settings->NHDT);
	setRegister(NCLT,settings->NCLT);
	setRegister(FDLT,settings->FDLT);
	setRegister(MHDPROXR,settings->MHDPROXR);
	setRegister(NHDPROXR,settings->NHDPROXR);
	setRegister(NCLPROXR,settings->NCLPROXR);
	setRegister(FDLPROXR,settings->FDLPROXR);
	setRegister(MHDPROXF,settings->MHDPROXF);
	setRegister(NHDPROXF,settings->NHDPROXF);
	setRegister(NCLPROXF,settings->NCLPROXF);
	setRegister(FDLPROXF,settings->FDLPROXF);
	setRegister(NHDPROXT,settings->NHDPROXT);
	setRegister(NCLPROXT,settings->NCLPROXT);
	setRegister(FDLPROXT,settings->FDLPROXT);
  	setRegister(DTR, settings->DTR);
	setRegister(AFE1, settings->AFE1);  
	setRegister(AFE2, settings->AFE2);
	setRegister(ECR, settings->ECR);
	setRegister(ACCR0, settings->ACCR0);
	setRegister(ACCR1, settings->ACCR1);
	setRegister(USL, settings->USL); 
	setRegister(LSL, settings->LSL); 
	setRegister(TL, settings->TL); 
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