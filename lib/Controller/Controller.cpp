#include "Controller.h"
#include <Arduino.h>

double sat(double in, double lower, double upper)
{
	if(lower>upper)
		return in;
	if(in>upper)
		return upper;
	else if(in < lower)
		return lower;
	return in;
}



Controller::Controller(float Kp, float Ki, float Kd, float ts,float lowerbound=1,float upperbound=-1)
{
	this->_Kp = Kp;
	this->_Ki = Ki;
	this->_Kd = Kd;

  	this->_ts = ts;

	this-> lower = lowerbound;
	this-> upper = upperbound;
  	
	this->_error = 0;
	this->_errorLast = 0;
	this->_iError = 0;
	this->_SP = 0;
}

float Controller::update(float nPV)
{
	_errorLast = _error;
	_error =_SP - nPV; 

	_dError = (_error - _errorLast)/_ts;
	_iError = sat(_iError+_ts*(_error),lower,upper);
	return(sat( _Kp*_error + _Ki*_iError + _Kd*_dError,lower,upper));
}

void Controller::reset()
{
	 this->_iError=0;
	 this->_errorLast= 0;
}

void Controller::setSP(float nSP)
{
	this->_SP=nSP;
	
}
double Controller::getError()
{
	return _error;
}

double Controller::getValue()
{
	return(sat( _Kp*_error + _Ki*_iError + _Kd*_dError,lower,upper));
}

double Controller::setBounds(double lower, double upper)
{
	this->upper = upper;
	this->lower = lower;
}
