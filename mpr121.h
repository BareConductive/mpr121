#ifndef MPR121
#define MPR121

class MPR121_t
{
	private:
		uint8_t address;
	public:
		void begin();
		void setI2Caddress();
		void enableTouchElectrodes();
		void run();
		void stop();
		void reset();
};

extern MPR121_t MPR121

#endif