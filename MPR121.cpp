extern "C" {
	#include <stdlib.h>
	#include <string.h>
	#include <inttypes.h>
}

#include "MPR121.h"
#include "MPR121_defs.h"
#include <Wire.h>

MPR121_t::MPR121_t(){
	address = 0x5A; // default address is 0x5A
	ECR_backup = 0x00;
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

volatile unsigned char MPR121_t::getRegister(unsigned char r){
    Wire.beginTransmission(address); 
    Wire.write(r); // set address register to read from our requested register
    Wire.endTransmission(false); // don't send stop so we can send a repeated start
    Wire.requestFrom(address,(unsigned char)1);  // just a single byte
    Wire.endTransmission();
    return Wire.read();
}

void MPR121_t::run(){
	setRegister(ECR, ECR_backup); 			   // restore backup to return to run mode
}

void MPR121_t::stop(){
	ECR_backup = getRegister(ECR);			   // backup ECR to restore when we enter run
	setRegister(ECR, getRegister(ECR) & 0xC0); // turn off all electrodes to enter stop
}

unsigned int MPR121_t::getTouchStatus(){
	return((unsigned int)getRegister(TS1) + ((unsigned int)getRegister(TS2)<<8));
}

bool MPR121_t::getTouchStatus(unsigned char electrode){
	if(electrode>11) return false; // avoid out of bound behaviour

	if(electrode<8){
		return((getRegister(TS1)>>electrode) & 0x01);
	} else {
		return((getRegister(TS2)>>(electrode-8)) & 0x01);;
	}
}


void MPR121_t::setTouchThreshold(unsigned char val){
	for(unsigned char i=0; i<13; i++){
		setTouchThreshold(val, i);
	}
}

void MPR121_t::setTouchThreshold(unsigned char electrode, unsigned char val){
	if(electrode>12) return; // avoid out of bounds behaviour
	
	setRegister(E0TTH + (electrode<<1), val); 	// this relies on the internal register
												// map of the MPR121 and uses <<1 as
												// a quick equivalent to x2
}

void MPR121_t::setReleaseThreshold(unsigned char val){
	for(unsigned char i=0; i<13; i++){
		setReleaseThreshold(val,i);
	}
}

void MPR121_t::setReleaseThreshold(unsigned char electrode, unsigned char val){
	if(electrode>12) return; // avoid out of bounds behaviour
	
	setRegister(E0RTH + (electrode<<1), val); 	// this relies on the internal register
													// map of the MPR121 and uses <<1 as
													// a quick equivalent to x2
}

void MPR121_t::pinMode(unsigned char electrode, pinf_t mode){
	if(electrode<4 || electrode >11) return; // only valid for ELE4..ELE11
											 //				   LED0..LED7
											 
	unsigned char bitmask = 1<<(electrode-4);											 
											 
	switch(mode){
		case INPUT:
			// EN = 1
			// DIR = 0
			// CTL0 = 0
			// CTL1 = 0
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) & ~bitmask);
			setRegister(CTL0, getRegister(DIR) & ~bitmask);
			setRegister(CTL1, getRegister(DIR) & ~bitmask);						
			break;
		case INPUT_PU:
			// EN = 1
			// DIR = 0
			// CTL0 = 1
			// CTL1 = 1
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) & ~bitmask);
			setRegister(CTL0, getRegister(DIR) | bitmask);
			setRegister(CTL1, getRegister(DIR) | bitmask);		
			break;
		case INPUT_PD:
			// EN = 1
			// DIR = 0
			// CTL0 = 1
			// CTL1 = 0
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) & ~bitmask);
			setRegister(CTL0, getRegister(DIR) | bitmask);
			setRegister(CTL1, getRegister(DIR) & ~bitmask);		
			break;		
		case OUTPUT:
			// EN = 1
			// DIR = 1
			// CTL0 = 0
			// CTL1 = 0
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) | bitmask);
			setRegister(CTL0, getRegister(DIR) & ~bitmask);
			setRegister(CTL1, getRegister(DIR) & ~bitmask);				
			break;		
		case OUTPUT_HS:
			// EN = 1
			// DIR = 1
			// CTL0 = 1
			// CTL1 = 1
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) | bitmask);
			setRegister(CTL0, getRegister(DIR) | bitmask);
			setRegister(CTL1, getRegister(DIR) | bitmask);						
			break;		
		case OUTPUT_LS:
			// EN = 1
			// DIR = 1
			// CTL0 = 1
			// CTL1 = 0
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) | bitmask);
			setRegister(CTL0, getRegister(DIR) | bitmask);
			setRegister(CTL1, getRegister(DIR) & ~bitmask);							
			break;			
	}
}

void MPR121_t::pinMode(unsigned char electrode, int mode){
	// this is to catch the fact that Arduino prefers its definition of INPUT and OUTPUT
	// to ours...
	
	if(mode == 1){
		pinMode(electrode, (pinf_t)OUTPUT);
	} else if(mode == 0){
		pinMode(electrode, (pinf_t)INPUT);
	} else {
		return; // anything that isn't a 1 or 0 is invalid
	}
}

void MPR121_t::digitalWrite(unsigned char electrode, unsigned char val){
	if(electrode<4 || electrode>11) return; // avoid out of bounds behaviour
	if(val){
		setRegister(SET, 1<<(electrode-4));
	} else {
		setRegister(CLR, 1<<(electrode-4));
	}
}

unsigned char MPR121_t::setNumDigPins(unsigned char numPins){
	if(numPins>8) numPins = 8; // maximum number of GPIO pins is 8 out of 12
	ECR_backup = (0x0F&(12-numPins)) | (ECR_backup&0xF0);
	//setRegister(ECR, ECR_backup);
	// if we are already running, update ECR
	// if not, just store the changes for when we hit run
	if(getRegister(ECR)&0x3F) setRegister(ECR, ECR_backup);
	//getRegister(ECR);
	//setRegister(ECR, ECR_backup);
	
	return(getRegister(ECR));
}

MPR121_t MPR121 = MPR121_t();