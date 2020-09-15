#ifndef CONTROLLER_H

#define CONTROLLER_H
class Controller
{
public:
	Controller(float Kp, float Ki, float Kd,float ts,float lowerbound,float upperbound);
	float update(float PV);
	void reset();
	void setSP(float);
	double getError();
	double getValue();
	double setBounds(double lower, double upper);

private:
	float _error;
	float _iError;
	float _dError;
	float _errorLast;

	float _SP;

	float _ts;
	float _Kp;
	float _Ki;
	float _Kd;

	double lower;
	double upper;

};


#endif
