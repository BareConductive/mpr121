#ifndef MPR121_H
#define MPR121_H

class MPR121_t
{
	private:
		unsigned char address;
		unsigned char touchThr[13];
		unsigned char releaseThr[13];
	public:
		MPR121_t();
		void begin(unsigned char address);	// I think you only need to be able to set the address
									 		// at the very beginning
		void begin(); 
		void run();
		void stop();
		void reset();
		void enableTouchElectrodes();
		
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