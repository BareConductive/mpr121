extern "C" {
	#include <stdlib.h>
	#include <string.h>
	#include <inttypes.h>
}

#include "MPR121.h"
#include <Arduino.h>

MPR121_t::MPR121_t(){
	Wire.begin();	
	address = 0x5A; // default address is 0x5A
	ECR_backup = 0x00;
	running = false;
	inited = false;
	error = NOT_INITED;
	touchData = 0;
	lastTouchData = 0;	
}

void MPR121_t::setRegister(unsigned char reg, unsigned char value){

	bool wasRunning = false;;

	if(reg==ECR){	// if we are modifying ECR, update our internal running status
		if(value&0x3F){
			running = true;
		} else {
			running = false;
		} 
	} else if(reg<CTL0){
		wasRunning = running;
		if(wasRunning) stop(); 	// we should ALWAYS be in stop mode for this unless 
								// modding ECR or GPIO / LED register
	}
	
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    if(Wire.endTransmission()!=0) error = ADDRESS_UNKNOWN;
    
    if(wasRunning) run();		// restore run mode if necessary
}

unsigned char MPR121_t::getRegister(unsigned char reg){
	unsigned char scratch;

    Wire.beginTransmission(address); 
    Wire.write(reg); // set address register to read from our requested register
    Wire.endTransmission(false); // repeated start
    Wire.requestFrom(address,(unsigned char)1);  // just a single byte
    if(Wire.endTransmission()!=0) error = ADDRESS_UNKNOWN;
    scratch = Wire.read();
    if(reg == TS2 && ((scratch&0x80)!=0)) error = OVERCURRENT_FLAG;
    if((reg == OORS1 || reg == OORS2) && (scratch!=0)) error = OUT_OF_RANGE;    
    return scratch;
}

bool MPR121_t::begin(){
	inited = true;	// always return true to show that we TRIED to init - the init error
					// should only tell us that we forgot to call begin()
	if(reset()){
		// default values...
		applySettings(&defaultSettings);
		return true;
	} else {
		return false;
	}
	
}

bool MPR121_t::begin(unsigned char address){
	if(address>=0x5A && address<=0x5D) // addresses only valid 0x5A to 0x5D 
	{
		this->address = address; // need to be specific here
	}
	return begin();
}

void MPR121_t::run(){
	if(!inited) return;
	setRegister(ECR, ECR_backup); 			   // restore backup to return to run mode
}

void MPR121_t::stop(){
	if(!inited) return;
	ECR_backup = getRegister(ECR);			   // backup ECR to restore when we enter run
	setRegister(ECR, ECR_backup & 0xC0); // turn off all electrodes to enter stop
}

bool MPR121_t::reset(){
	// return true if we successfully reset a device at the 
	// address we are expecting
	
	// AFE2 is one of the few registers that defaults to a non-zero value - checking it 
	// is sensible reading back an incorrect value implies something went wrong - we also 
	// check TS2 bit 7 to see if we have an overcurrent flag set
	
	setRegister(SRST, 0x63); // soft reset
	if(error == ADDRESS_UNKNOWN) return(false); 	// stops us overwriting the address 
													// error with anything else
	if(getRegister(AFE2)!=0x24){
		error = READBACK_FAIL;
		return(false);
	} else if((getRegister(TS2)&0x80)!=0){
		error = OVERCURRENT_FLAG;
		return(false);
	} else {
		return(true);
	}	
}

void MPR121_t::applySettings(MPR121_settings *settings){
	bool wasRunning = running;
	if(wasRunning) stop();  // can't change most regs when running - checking here avoids
							// multiple stop() / run() calls

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
	setRegister(ACCR0, settings->ACCR0);
	setRegister(ACCR1, settings->ACCR1);
	setRegister(USL, settings->USL); 
	setRegister(LSL, settings->LSL); 
	setRegister(TL, settings->TL); 
	
	setRegister(ECR, settings->ECR);	
	
	inited=true;
	
	if(error == NOT_INITED) error = NO_ERROR; 	// if our only error is that we are not 
												// inited, clear the error
	setTouchThreshold(settings->TTHRESH);
	setReleaseThreshold(settings->RTHRESH);
	setInterruptPin(settings->INTERRUPT);	
	
	if(wasRunning) run();	
}

mpr121_error_t MPR121_t::getError(){
	if(!inited) return(NOT_INITED);
	// returns only the most recent error - each error overwrites the previous one
	if(error==NO_ERROR){		// if we don't have an error, check for OOR on both
		getRegister(OORS1);		// OOR registers - we may not have read them yet
		getRegister(OORS2);		// (whereas the other errors should have been caught)
	}
	return(error);
}

bool MPR121_t::isRunning(){
	return running;
}

bool MPR121_t::isInited(){
	return inited;
}

void MPR121_t::updateTouchData(){
	if(!inited) return;
	
	lastTouchData = touchData;
	touchData = (unsigned int)getRegister(TS1) + ((unsigned int)getRegister(TS2)<<8);
}

bool MPR121_t::getTouchData(unsigned char electrode){
	if(electrode>12 || !inited) return false; // avoid out of bounds behaviour

	return((touchData>>electrode)&1);
}

bool MPR121_t::getLastTouchData(unsigned char electrode){
	if(electrode>12 || !inited) return false; // avoid out of bounds behaviour

	return((lastTouchData>>electrode)&1);
}

void MPR121_t::updateFilteredData(){
	if(!inited) return;
	unsigned char LSB, MSB;

    Wire.beginTransmission(address); 
    Wire.write(E0FDL); // set address register to read from the start of the filtered data
    Wire.endTransmission(false); // don't send stop so we can send a repeated start
  
    if(Wire.requestFrom(address,(unsigned char)26)==26){
		for(int i=0; i<13; i++){ // 13 filtered values
		  LSB = Wire.read();
		  MSB = Wire.read();
		  filteredData[i] = ((MSB << 8) | LSB);
		}     
    } else {
		for(int i=0; i<13; i++){         
		  filteredData[i] = 0xFFFF; // this is an invalid filtered value indicating error
		}        
    }
}

int MPR121_t::getFilteredData(unsigned char electrode){
	if(electrode>12 || !inited) return(0xFFFF); // avoid out of bounds behaviour

	return(filteredData[electrode]);
}

void MPR121_t::updateBaselineData(){
	if(!inited) return;

    Wire.beginTransmission(address); 
    Wire.write(E0BV); // set address register to read from the start of the baseline data
    Wire.endTransmission(false); // don't send stop so we can send a repeated start
  
    if(Wire.requestFrom(address,(unsigned char)13)==13){
		for(int i=0; i<13; i++){ // 13 filtered values
		  baselineData[i] = Wire.read()<<2;
		}     
    } else {
		for(int i=0; i<13; i++){         
		  baselineData[i] = 0xFFFF; // this is an invalid filtered value, indicating an error
		}        
    }
}

int MPR121_t::getBaselineData(unsigned char electrode){
	if(electrode>12 || !inited) return(0xFFFF); // avoid out of bounds behaviour
  
	return(baselineData[electrode]);
}

bool MPR121_t::isNewTouch(unsigned char electrode){
	if(electrode>12 || !inited) return(false); // avoid out of bounds behaviour	
	return((getLastTouchData(electrode) == false) && (getTouchData(electrode) == true));
}

bool MPR121_t::isNewRelease(unsigned char electrode){
	if(electrode>12 || !inited) return(false); // avoid out of bounds behaviour	
	return((getLastTouchData(electrode) == true) && (getTouchData(electrode) == false));
}

void MPR121_t::updateAll(){
	updateTouchData();
	updateBaselineData();
	updateFilteredData();
}

void MPR121_t::setTouchThreshold(unsigned char val){
	if(!inited) return;
	bool wasRunning = running;
	
	if(wasRunning) stop();	// can only change thresholds when not running
							// checking here avoids multiple stop() / run() calls
	
	for(unsigned char i=0; i<13; i++){
		setTouchThreshold(i, val);
	}
	
	if(wasRunning) run();
}

void MPR121_t::setTouchThreshold(unsigned char electrode, unsigned char val){
	if(electrode>12 || !inited) return; // avoid out of bounds behaviour
	
	setRegister(E0TTH + (electrode<<1), val); 	// this relies on the internal register
												// map of the MPR121 and uses <<1 as
												// a quick equivalent to x2																						
}

void MPR121_t::setReleaseThreshold(unsigned char val){
	if(!inited) return;
	bool wasRunning = running;
	
	if(wasRunning) stop();	// can only change thresholds when not running
							// checking here avoids multiple stop / starts
	
	for(unsigned char i=0; i<13; i++){
		setReleaseThreshold(i,val);
	}
	
	if(wasRunning) run();
}

void MPR121_t::setReleaseThreshold(unsigned char electrode, unsigned char val){
	if(electrode>12 || !inited) return; // avoid out of bounds behaviour

	setRegister(E0RTH + (electrode<<1), val); 	// this relies on the internal register
													// map of the MPR121 and uses <<1 as
													// a quick equivalent to x2
}

unsigned char MPR121_t::getTouchThreshold(unsigned char electrode){
	if(electrode>12 || !inited) return(0xFF); // avoid out of bounds behaviour
	return(getRegister(E0TTH+(electrode<<1)));
}
unsigned char MPR121_t::getReleaseThreshold(unsigned char electrode){
	if(electrode>12 || !inited) return(0xFF); // avoid out of bounds behaviour
	return(getRegister(E0RTH+(electrode<<1)));
}

void MPR121_t::setInterruptPin(unsigned char pin){
	// :: here forces the compiler to use Arduino's pinMode, not ours within the class
	if(!inited) return;
	::pinMode(pin, INPUT_PULLUP);
	interruptPin = pin;		
	
}

bool MPR121_t::touchStatusChanged(){
	// :: here forces the compiler to use Arduino's digitalRead, not ours within the class
	return(!::digitalRead(interruptPin));
}

void MPR121_t::setProxMode(mpr121_proxmode_t mode){

	if(!inited) return;

	bool wasRunning = running;

	if(wasRunning) stop();

	switch(mode){
		case DISABLED:
			ECR_backup &= ~(3<<4);	// ELEPROX[0:1] = 00
			break;
		case PROX0_1:
			ECR_backup |=  (1<<4);	// ELEPROX[0:1] = 01
			ECR_backup &= ~(1<<5);			
			break;
		case PROX0_3:
			ECR_backup &= ~(1<<4);	// ELEPROX[0:1] = 10
			ECR_backup |=  (1<<5);			
			break;
		case PROX0_11:
			ECR_backup |=  (3<<4);	// ELEPROX[0:1] = 11
			break;
	}
	
	if(wasRunning) run();
}

void MPR121_t::setNumDigPins(unsigned char numPins){
	if(!inited) return;
	bool wasRunning = running;

	if(numPins>8) numPins = 8; // maximum number of GPIO pins is 8 out of 12
	
	if(wasRunning){
		stop(); // have to stop to change ECR
	}
	ECR_backup = (0x0F&(12-numPins)) | (ECR_backup&0xF0);
	if(wasRunning){
		run();
	}
	
}

void MPR121_t::pinMode(unsigned char electrode, mpr121_pinf_t mode){
	if(electrode<4 || electrode >11 || !inited) return; // only valid for ELE4..ELE11
											 			//				  LED0..LED7
											 
	unsigned char bitmask = 1<<(electrode-4);											 
											 
	switch(mode){
		case INPUT_PU:
			// EN = 1
			// DIR = 0
			// CTL0 = 1
			// CTL1 = 1
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) & ~bitmask);
			setRegister(CTL0, getRegister(CTL0) | bitmask);
			setRegister(CTL1, getRegister(CTL1) | bitmask);		
			break;
		case INPUT_PD:
			// EN = 1
			// DIR = 0
			// CTL0 = 1
			// CTL1 = 0
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) & ~bitmask);
			setRegister(CTL0, getRegister(CTL0) | bitmask);
			setRegister(CTL1, getRegister(CTL1) & ~bitmask);		
			break;		
		case OUTPUT_HS:
			// EN = 1
			// DIR = 1
			// CTL0 = 1
			// CTL1 = 1
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) | bitmask);
			setRegister(CTL0, getRegister(CTL0) | bitmask);
			setRegister(CTL1, getRegister(CTL1) | bitmask);						
			break;		
		case OUTPUT_LS:
			// EN = 1
			// DIR = 1
			// CTL0 = 1
			// CTL1 = 0
			setRegister(EN, getRegister(EN) | bitmask);
			setRegister(DIR, getRegister(DIR) | bitmask);
			setRegister(CTL0, getRegister(CTL0) | bitmask);
			setRegister(CTL1, getRegister(CTL1) & ~bitmask);							
			break;			
	}
}

void MPR121_t::pinMode(unsigned char electrode, int mode){
	if(!inited) return;
	
	// this is to catch the fact that Arduino prefers its definition of INPUT and OUTPUT
	// to ours...
	
	unsigned char bitmask = 1<<(electrode-4);	
	
	if(mode == OUTPUT){ 
		// EN = 1
		// DIR = 1
		// CTL0 = 0
		// CTL1 = 0
		setRegister(EN, getRegister(EN) | bitmask);
		setRegister(DIR, getRegister(DIR) | bitmask);
		setRegister(CTL0, getRegister(CTL0) & ~bitmask);
		setRegister(CTL1, getRegister(CTL1) & ~bitmask);				
			
	} else if(mode == INPUT){
		// EN = 1
		// DIR = 0
		// CTL0 = 0
		// CTL1 = 0
		setRegister(EN, getRegister(EN) | bitmask);
		setRegister(DIR, getRegister(DIR) & ~bitmask);
		setRegister(CTL0, getRegister(CTL0) & ~bitmask);
		setRegister(CTL1, getRegister(CTL1) & ~bitmask);						
	} else {
		return; // anything that isn't a 1 or 0 is invalid
	}
}

void MPR121_t::digitalWrite(unsigned char electrode, unsigned char val){
	if(electrode<4 || electrode>11 || !inited) return; // avoid out of bounds behaviour
	if(val){
		setRegister(SET, 1<<(electrode-4));
	} else {
		setRegister(CLR, 1<<(electrode-4));
	}
}

void MPR121_t::digitalToggle(unsigned char electrode){
	if(electrode<4 || electrode>11 || !inited) return; // avoid out of bounds behaviour
	setRegister(TOG, 1<<(electrode-4));	
}

bool MPR121_t::digitalRead(unsigned char electrode){
	if(electrode<4 || electrode>11 || !inited) return false; // avoid out of bounds behaviour
	return(((getRegister(DAT)>>(electrode-4))&1)==1);
}

void MPR121_t::analogWrite(unsigned char electrode, unsigned char value){
	// LED output 5 (ELE9) and output 6 (ELE10) have a PWM bug
	// https://community.freescale.com/thread/305474

	if(electrode<4 || electrode>11 || !inited) return; // avoid out of bounds behaviour
	
	unsigned char shiftedVal = value>>4;	
	
	if(shiftedVal > 0){
		setRegister(SET, 1<<(electrode-4)); // normal PWM operation 
	} else {
		setRegister(CLR, 1<<(electrode-4)); // this make a 0 PWM setting turn off the output	
	}
	
	unsigned char scratch;
  
	switch(electrode-4){

	case 0:
	  scratch = getRegister(PWM0);
	  setRegister(PWM0, (shiftedVal & 0x0F) | (scratch & 0xF0));
	  break;
	case 1:
	  scratch = getRegister(PWM0);
	  setRegister(PWM0, ((shiftedVal & 0x0F)<<4) | (scratch & 0x0F));
	  break;
	case 2:
	  scratch = getRegister(PWM1);
	  setRegister(PWM1, (shiftedVal & 0x0F) | (scratch & 0xF0));
	  break;
	case 3:
	  scratch = getRegister(PWM1);
	  setRegister(PWM1, ((shiftedVal & 0x0F)<<4) | (scratch & 0x0F));
	  break;
	case 4:
	  scratch = getRegister(PWM2);
	  setRegister(PWM2, (shiftedVal & 0x0F) | (scratch & 0xF0));
	  break;
	case 5:
	  scratch = getRegister(PWM2);
	  setRegister(PWM2, ((shiftedVal & 0x0F)<<4) | (scratch & 0x0F));
	  break;
	case 6:
	  scratch = getRegister(PWM3);
	  setRegister(PWM3, (shiftedVal & 0x0F) | (scratch & 0xF0));
	  break;
	case 7:
	  scratch = getRegister(PWM3);
	  setRegister(PWM3, ((shiftedVal & 0x0F)<<4) | (scratch & 0x0F));
	  break;
	}

	
}

MPR121_t MPR121 = MPR121_t();