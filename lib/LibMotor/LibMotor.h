/* 
 * LibMotor
 * Library for L298N dual H-Bridge
 *
 */

#ifndef LibMotor_h

#define LibMotor_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "../Encoder/Encoder.h"

//Class for L298N Dual H-Bridge
class Mdrive{
  public:
	Mdrive(){}
	Mdrive(int out1, int out2);
	void init(float maxMvolt, float maxBvolt);
	void setPWM(uint8_t dutycycle);
	void stop();
	void setVoltage(float refVolt);
	virtual void enable(int enA, bool value);
	
  private:
	int _out1;	
	int _out2;    
	int _enA;
	float _bVolt;
	float _mVolt;
	float _refVolt;
	int _duty;
    
};

//Class for wheel drive: reads encoder and controls motor
class WheelDrive : public Mdrive{
	public:
		WheelDrive();
		WheelDrive(int out1, int out2,int cha,int chb,int radius=0,double radpp=0);
	    void setRadius(int radius);
        double getW();
        double getV();
		double getDeltaTheta();
		double getDeltaCount();
		void resetTheta();
        void update(double dT);
		void get_count(long * count);
	private:	
		Encoder *knob; 	
		long knobLast;
		long knobNow;
        int radius;// radius in desired unit (will determine V)
        double W;
		double radpp;
};

#endif
