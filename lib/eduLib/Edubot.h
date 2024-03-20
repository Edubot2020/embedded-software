#ifndef EDUBOT_H
#define EDUBOT_H
#endif

// Dependencies (should be in arduino's library directory)
#include "../LibMotor/LibMotor.h"
#include "../Encoder/Encoder.h"
#include "../LibSonar/LibSonar.h"
#include "../Controller/Controller.h"

// Pinout for your robot
//  The default Pinout.h provided defines EduBot's default connections
#include "Pinout.h"


//----------------*** Library Definitions ***------------------

// *** Encoder params:

// 1496 = pulses per revolution
// 360/1496 = degrees per pulse
// 2pi/1496 = radians per pulse
#define PPR 1496
#define degPP 0.2406
#define radPP 0.0042

// *** Mechanical params:

// R = Wheel radius,
// L = Distance between wheels
// EDU_RL = R/L should be calculated and put here literally to save computation time
#define EDU_R 3.2
#define EDU_L 16.2
#define EDU_RL 0.1975


// *** Controller params:

// Minimum delta error for rotational control (in radians)
// If error varies less than this between iterations, consider 
// that the robot has finished its rotation
#define DEL_ERROR 1
// Maximum linear velocity
#define EDU_VMAX 50

// Sample time
#define TS 0.01
// Output bounds
#define OUTPUT_BOUND 6

// The following block defines the controllers to be used
// There are two topologies implemented in this file:
// ---> Individual Motors
// ---  One PID controller for the left and one for the right motor
// ---  Computes the Set Point for each based on the desired linear/angular velocity
// ---  Tends to not work very well when the robot is disturbed (this claim is purely experimental)
//
// ---> Coupled Motors
// ---  One PID controller for linear velocity, one for angular velocity
// ---  Tends to work better when the robot is disturbed, and during acceleration
// TODO: write this up, compare performances, and put the link here
// 
// To change topology, just use #define EDU_CONTROL_(TOPOLOGY_USED)
// To implement a new topology or controller, add the definition to the list
// below and change update_control and update_setPoint() functions accordingly
//

// This defines EDU_CONTROL_COUPLED_MOTORS as the standard
#ifndef EDU_CONTROL_INDIVIDUAL_MOTORS
#ifndef EDU_CONTROL_COUPLED_MOTORS

#define EDU_CONTROL_COUPLED_MOTORS

#endif
#endif

#ifdef EDU_CONTROL_INDIVIDUAL_MOTORS
// PID Controllers for right and left motors:
#define KPRIGHT 0.6 
#define KIRIGHT 5
#define KDRIGHT 0 
Controller controlRight (KPRIGHT, KIRIGHT, KDRIGHT,TS,-OUTPUT_BOUND,OUTPUT_BOUND);
#define KPLEFT 0.6 
#define KILEFT 5
#define KDLEFT 0
Controller controlLeft  (KPLEFT,  KILEFT,  KDLEFT, TS,-OUTPUT_BOUND,OUTPUT_BOUND);
#endif

#ifdef EDU_CONTROL_COUPLED_MOTORS
// PID Controllers for linear and angular velocity control:
#define KPV 0.225
#define KIV 2.25
#define KDV 0 
Controller controlV (KPV ,  KIV ,  KDV , TS,-OUTPUT_BOUND,OUTPUT_BOUND);
#define KPW 2
#define KIW 20
#define KDW 0
Controller controlW (KPW , KIW , KDW ,TS,-OUTPUT_BOUND,OUTPUT_BOUND);
#endif

// PID Controller for rotational control:
#define KPTHETA 0.5  //0.5
#define KITHETA 0.5   //0.0
#define KDTHETA 0.03 //0.03 
Controller controlTheta (KPTHETA, KITHETA, KDTHETA,TS,-OUTPUT_BOUND,OUTPUT_BOUND);

//----------------*** Object Initialization ***------------------
// WheelDrives for each wheel (two digital pins for H-Bridge, two for encoder)
WheelDrive wheelRight(RIGHT_BRIDGE_A, 
					  RIGHT_BRIDGE_B,
					  RIGHT_ENCODER_A,
					  RIGHT_ENCODER_B,
					  EDU_R,radPP);
WheelDrive wheelLeft(LEFT_BRIDGE_A, 
					  LEFT_BRIDGE_B,
					  LEFT_ENCODER_A,
					  LEFT_ENCODER_B,
					  EDU_R,radPP);

//----------------*** Global Variables ***------------------

// Turns controllers on or off 
// TODO: Fix this implementation (turn off the controller objects and fix the update method
// , don't use this bool...)
bool control_on=false; 

// Counts to 80 within timer2 ISR to update at 100 Hz
// TODO: get timer2 to generate ISR at 1/TS (100 Hz) without breaking everything else
int count =0;      

//----------------*** Function Prototypes ***------------------

// *** "Top-Level": these functions must be executed for everything else to work.
void edu_update(); // Run at sample rate (Inside ISR)
void edu_setup();  // Run once at startup

// *** "Library-Level": used inside this .h file to abstract implementations
void update_control();
void readMotorData();
void setup_timer2();
void update_setPoint(double v, double w);

// *** Movement Functions ("user-level"):
void edu_stop();              // Stops the motors (open-loop)
void edu_controlledStop();    // Stops the robot (closed-loop)
void edu_rotate(double degs); // Rotates the robot by degs degrees
void edu_moveLine(double v);  // Moves the robot in a straight line
void edu_moveVW(double v, double w); // Moves the robot with specified linear and angular speed

// *** Utility functions 

// Saturation between lower and upper
double saturate(double in, double lower, double upper);

// Speed conversion  (v = linear velocity, w = angular velocity)
// robot (v,w) -> motor w (left, right)
double computeWl(double v, double w);
double computeWr(double v, double w);
// motor w (left, right) -> robot (v,w) 
double getW(double Wl, double Wr);
double getV(double Wl, double Wr);


//----------------*** Function Definitions  ***------------------

double saturate(double in, double lower, double upper)
{
	if(lower>upper)
		return in;
	if(in>upper)
		return upper;
	else if(in < lower)
		return lower;
	return in;	
}

double getV(double Wl, double Wr)
{
	return (Wl+Wr)*EDU_R/2;
}

double getW(double Wl, double Wr)
{
	return (Wl-Wr)*EDU_RL;
}

double computeWr(double v, double w)
{
	return ( v/EDU_R - (0.5*w/EDU_RL) );
}

double computeWl(double v, double w)
{
	return ( v/EDU_R + (0.5*w/EDU_RL) );
}

void edu_controlledStop()
{
	edu_moveVW(0,0);
	delay(100);
	control_on = false; //era true
}
void edu_stop()
{
	edu_controlledStop();
}

void edu_moveVW(double v, double w)
{
	update_setPoint(v,w);
	control_on = true;
}

void edu_moveLine(double v)
{
	edu_moveVW(v,0);
	control_on = true;
}

void edu_rotate(double degs)
{
	double theta=0;
	char ccount=0;
	
	edu_stop();delay(300);

	controlTheta.setSP(degs); 

	control_on = false; // TODO: fix this confusing declaration
						// control_on=false does not turn off controlTheta,
						// only whatever is inside update_control()
	
	
	unsigned long startMillis = 0;	
	startMillis = millis();
	
	while((ccount < 10) && (millis() - startMillis <= 1000))
	{
		// getW is used to convert delta theta from the wheels into delta
		// theta for the robot
		theta+=degPP*getW(wheelLeft.getDeltaCount(),wheelRight.getDeltaCount());

		// TODO: implement a better controlTheta.
		// This one is modelled considering differential voltage between
		// the motors, but it can generate residual linear velocity
		double V = controlTheta.update(theta) * 0.5;
		wheelRight.setVoltage(-V);
		wheelLeft.setVoltage(V);

		// Stops this loop whenever the error varies less than DEL_ERROR for a while
		if(fabs(controlTheta.getError()) < DEL_ERROR)
			ccount++;
		else
			ccount=0;
		// Delay has to be here to avoid competition between this loop and the ISR
		delay(10);
	 }
	 edu_stop();delay(300);
}

void edu_wheel(double left, double right)
{
	if (left > 1.0)
		left = 1.0;
	else if (left < -1.0)
		left = -1.0;

	if (right > 1.0)
		right = 1.0;
	else if (right < -1.0)
		right = -1.0;

	wheelLeft.setVoltage(maxMvolt*left);
	wheelRight.setVoltage(maxMvolt*right);
	
	control_on = false;
}


void setup_timer2()
{
  	control_on=false;
  	// This is stolen from https://www.instructables.com/id/Arduino-Timer-Interrupts/
  	// TODO:Implement a better config for timer2 ... sorry!

  	TCCR2A = 0;// set entire TCCR2A register to 0
  	TCCR2B = 0;// same for TCCR2B
  	TCNT2  = 0;//initialize counter value to 0
  	// set compare match register for 8khz increments
  	OCR2A = 249; //8kHz = (16*10^6) / (8000*8) - 1 (must be <256)
  	// turn on CTC mode
  	TCCR2A |= (1 << WGM21);
  	// Set CS21 bit for 8 prescaler
  	TCCR2B |= (1 << CS21);   
  	// enable timer compare interrupt
  	TIMSK2 |= (1 << OCIE2A);
}

void edu_setup()
{
  	wheelLeft.init(maxMvolt, maxBvolt);
  	wheelRight.init(maxMvolt, maxBvolt);
  	setup_timer2();
}

void readMotorData()
{
	wheelLeft.update(TS);
	wheelRight.update(TS);
}


void update_control()
{
	// TODO: long stack, should be a better way to do this without more variables
	// Sets the voltage of each motor
	// to the value calculated by the controller
	// considering the most recent angular speed of the motor
	
#ifdef EDU_CONTROL_INDIVIDUAL_MOTORS
	controlLeft.update(wheelLeft.getW());
	controlRight.update(wheelRight.getW());

	wheelLeft.setVoltage(controlLeft.getValue());
	wheelRight.setVoltage(controlRight.getValue());
#endif

#ifdef EDU_CONTROL_COUPLED_MOTORS
	controlV.update(getV(wheelLeft.getW(),wheelRight.getW()));
	controlW.update(getW(wheelLeft.getW(),wheelRight.getW()));

	wheelLeft.setVoltage(controlV.getValue()+controlW.getValue());
	wheelRight.setVoltage(controlV.getValue()-controlW.getValue());
#endif
}

void update_setPoint(double v, double w)
{
#ifdef EDU_CONTROL_INDIVIDUAL_MOTORS
	controlLeft.setSP(computeWl(v,w));
	controlRight.setSP(computeWr(v,w));
#endif
#ifdef EDU_CONTROL_COUPLED_MOTORS
	controlW.setSP(w);
	controlV.setSP(v); 
#endif
}

void edu_update()
{
	// Anything inside this function runs at 100 Hz regardless 
	// Do NOT put delays or loops here! 
	// This should execute quickly

	readMotorData(); // Updates angular speed of each motor
	if(control_on) // If the controllers are on, update the motor voltages
	{
		update_control();
	}
}


ISR(TIMER2_COMPA_vect){//timer2 interrupt @ 8kHz
	// counts to 80 to reduce the frequency.
	// I know, sorry...
	// TODO: run the ISR at 100 Hz to save time
    count++;
    if(count>80)
	{
		count = 0;
		edu_update();
  	}
}

void get_counts(long * left, long * right){

wheelLeft.get_count(left);
wheelRight.get_count(right);
}


