#ifndef MPR121_H
#define MPR121_H

// idea behind this is to create a settings structure that we can use to store all the
// setup variables for a particular setup - comes pre-instantiated with defaults and
// can be easily tweaked - we pass by reference (as a pointer) to save RAM
struct MPR121_settings
{
	
};

class MPR121_t
{
	private:
		unsigned char address;
		unsigned char touchThr[13];
		unsigned char releaseThr[13];
	public:
		MPR121_t();
		void begin(unsigned char address);	// I think you only need to be able to set the
											// address at the very beginning
		void begin(); 
		void run();
		void stop();
		void reset();
		void enableTouchElectrodes();
		void applySettings(MPR121_settings *settings);
		
		void setTouchThreshold(unsigned char val);
		void setTouchThreshold(unsigned char val, unsigned char electrode);
		void setReleaseThreshold(unsigned char val);
		void setReleaseThreshold(unsigned char val, unsigned char electrode);		
		
		bool getTouchStatus(unsigned char electrode);
		unsigned int getTouchStatus();
		
		void setRegister(unsigned char r, unsigned char v);
		unsigned char getRegister(unsigned char r);


};

extern MPR121_t MPR121;

#endif