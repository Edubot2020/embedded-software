#ifndef Pinos_h
#define Pinos_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*
 * Updated pinout for Edubot 
*/

/*
	Contact Switches
*/
#define CFL A0	// Contact switch Front-Left
#define CFR A1	// Contact switch Front-Right
#define CBL A2	// Contact switch Back-Left
#define CBR A3	// Contact switch Back-Right
/*
   Pins used for the Sonars
*/
#define ECHOF 7  //Echo Sonar Front
#define TRIGF 8 //Trig Sonar  Front
#define ECHOR 12 //Echo Sonar Right
#define TRIGR 13 //Trig Sonar Right
#define ECHOL A5 //Echo Sonar Left
#define TRIGL A4 //Trig Sonar Left
/*
   H-Bridge pins
   Should follow this standard:
   ->Positive voltage on both motors moves the robot forward
   ->Check: Clockwise rotation-> motor left has positive voltage, motor right has negative voltage
*/
#define RIGHT_BRIDGE_A 5 
#define RIGHT_BRIDGE_B 6
#define LEFT_BRIDGE_A 10
#define LEFT_BRIDGE_B 9
/* 
 * Encoder pins
 * Should follow this standard:
 *  ->Positive encoder count on both motors  moves the robot forward
 *  ->Check: Clockwise rotation-> motor left has positive count, motor right has negative count
 */
#define RIGHT_ENCODER_A 2
#define RIGHT_ENCODER_B 11
#define LEFT_ENCODER_A 3
#define LEFT_ENCODER_B 4
/*
   Max Motor voltage and Max. Battery  voltage (steps down with the H-Bridge)
*/
#define maxMvolt 6
#define maxBvolt 6

#endif
