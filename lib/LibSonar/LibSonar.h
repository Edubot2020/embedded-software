/*
 * Interface for sonars for use with eduBot
*/


#ifndef LibSonar_h
#define LibSonar_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Sonar{
	public:
	Sonar(){}
	Sonar(int _TRIG, int _ECHO); 
	Sonar(int _TRIG, int _ECHO, long _TOUT);
	double measureDist();
	private:
	int Trig_pin;
	int Echo_pin;
	long Time_out;
	long measureTime();
};

#endif
