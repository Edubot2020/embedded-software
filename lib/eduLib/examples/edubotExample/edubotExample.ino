/**
 * Example code for Edubot Software
 * 
 * Edubot is an open-source mobile robot for education. 
 *
 * This example is aimed at introduction-level students as an introduction to the library. 
 * Edubot.h provides easy interfaces to control the robot, and is maintained by advanced students.
 * 
 * Feedback control is necessary to control the motors and the robot's movement, and
 * it has been implemented in Edubot.h. 
 *
 * *------- Movement Functions: ---------*
 * 
 * ----> edu_stop(): stops the robot
 * ----> edu_moveLine(double speed): moves the robot in a straight line with specified speed (cm/s)
 * ----> edu_rotate(double degs): rotates the robot by the specified degrees
 * ----> edu_moveVW(double V, double W): moves the robot with specified linear and angular speeds
 * 
 * Linear movement is positive towards the front of the robot;
 * Rotation is positive in the clockwise direction;
 * 
 * *-------------------------------------*
 */
#include <Edubot.h>

// Arduino IDE won't link these dependencies from Edubot.h, so put them here...
#include <LibMotor.h>
#include <Encoder.h>
#include <LibSonar.h>
#include <Controller.h>

//     *** Global Variables: ***

// Sonars mounted on the robot.
// Usage example: sonarRight.medeDistancia() returns the distance measured by sonarRight
Sonar sonarRight(TRIGR, ECHOR);
Sonar sonarFront(TRIGF, ECHOF);
Sonar sonarLeft(TRIGL, ECHOL);

// Distances measured in each direction. Stored in variables to save time
long distR, distF,distL;


//     *** Function Prototypes:  ***

void measureDists();
void serialSendDists();

void setup() {
  
  Serial.begin(9600);

  // Sets up whatever is necessary for the library to work
  // should always be called at setup!
  edu_setup();
  
}


void loop() {

	// Measure sonar distances only once and store them
	measureDists();

	// Move in a straight line with 80% of max. speed
  	edu_moveLine(0.8*EDU_VMAX);

	// Only stop if ....

	// ---> Distance in front is less than 7 cm
  	if(distF<7)
  	{
		// Stops, moves back, and spins around a bit to show off
    	edu_stop();
    	delay(500);
	
    	edu_moveLine(-EDU_VMAX);
    	delay(500);

    	edu_stop();
    	delay(500);

    	edu_rotate(180);
    	edu_rotate(-90);
  	}
	// ---> or one of the contact switches has been hit
	// (contact switches: CFR (front right), CFL (front left), 
	// CBR (back right), CBL ( back left)
  	else if(digitalRead(CFR)==LOW || digitalRead(CFL)==LOW)
  	{
		// Stops, moves back and turns around
    	edu_stop(); 
		edu_moveLine(-EDU_VMAX);
		delay(500);
		edu_rotate(180);
		}  
}

// *** Function definitions: ***
void measureDists()
{
  distR = sonarRight.measureDist();
  distL = sonarLeft.measureDist();
  distF = sonarFront.measureDist();
}

void serialSendDists()
{
  Serial.print("Distance Right: ");Serial.println(distR);
  Serial.print("Distance Left: ");Serial.println(distL);
  Serial.print("Distance Front: ");Serial.println(distF);
}
