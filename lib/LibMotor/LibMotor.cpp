/* 
*
 * LibMotor
 * Library for L298N dual H-Bridge
 *
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "LibMotor.h"
Mdrive::Mdrive(int out1, int out2){	
	pinMode(out1, OUTPUT);
	pinMode(out2, OUTPUT);
	
	this->_out1 = out1;
	this->_out2 = out2;

}

void Mdrive::init(float maxMvolt, float maxBvolt){
	this->_bVolt = maxBvolt;
	this->_mVolt = maxMvolt;
	stop();
}

void Mdrive::setVoltage(float refVolt){
	
	if(refVolt <= _mVolt && refVolt >= -_mVolt){
		_duty = 255*(_bVolt + refVolt)/(_bVolt*2);
	}
	else if(refVolt > _mVolt){
		_duty = 255*((_bVolt +_mVolt)/_bVolt*2);
	}
	else if(refVolt < -_mVolt){
		_duty = ((_bVolt-_mVolt)/_bVolt*2);
	}
	
	this->_refVolt = refVolt;
	
	setPWM(_duty);
}

void Mdrive::setPWM(uint8_t dutycycle){
	
	analogWrite(_out1, dutycycle);
	analogWrite(_out2, 255-dutycycle);
}

void Mdrive::stop(){
	setVoltage(0);
}

void Mdrive::enable(int enA, bool value){
	this->_enA = enA;
	
	pinMode(_enA, OUTPUT);
	digitalWrite(_enA, value);

}
// Constructor:
WheelDrive::WheelDrive():radius(0),knobLast(0),knobNow(0),radpp(0){
		

}
WheelDrive::WheelDrive(int out1, int out2,int cha,int chb,int radius,double radpp):Mdrive(out1,out2){
	this->knob = new Encoder(cha,chb);
	knob->write(0);
	this->radius = radius;
	knobLast=0;knobNow=0;
	this->radpp=radpp;
}
// Definitions:

void WheelDrive::update(double dT){
	knobLast=knobNow;
	knobNow = this->knob->read();
	this->W =radpp*(knobNow-knobLast)/dT;
}

double WheelDrive::getV(){
	return this->W*this->radius;
}

double WheelDrive::getW(){
	return this->W;
}

void WheelDrive::setRadius(int radius){
		this->radius=radius;
}

double WheelDrive::getDeltaTheta(){
		return this->radpp*(this->knobNow - this->knobLast);
}

double WheelDrive::getDeltaCount(){
		return (this->knobNow - this->knobLast);
}

void WheelDrive::get_count(long * count){

  *count = this->knob->read();
}
