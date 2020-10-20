#include <Edubot.h>
#include <LibMotor.h>
#include <Encoder.h>
#include <Controller.h>

#define LOW_BATTERY 7

long int dt                   =   0;
long int leftCounter          =   0;
long int rightCounter         =   0;
long int leftCounterPrevious  =   0;
long int rightCounterPrevious =   0;
double x                      = 0.0;
double y                      = 0.0;
double theta                  = 0.0;

double divisor                = 0.49925;
double batCell0               = 0.0;
double batCell1               = 0.0;
double batCell2               = 0.0;
boolean state                 = false; 

String dataSend               =  "";

long int time_prev = millis();

double diameterWheel     =  0.065; //(unit meter)
double pulsesPerRotation = 1496.0; //PPR
double dBaseline         =  0.165; //distance between the left and right wheels (unit meters)
double circleWheel = 2*PI*diameterWheel/2.0;
double metersPerPulse = circleWheel/pulsesPerRotation;

unsigned long startMillisBattery = 0;
const unsigned long periodBattery = 500;

unsigned long startMillis = 0;
const unsigned long period = 100;

void sendRobotOdometry();
void receiveRequestCommand();
void decodeCommand(String message);
void calculateOdometry();

void setup() {
    pinMode(LOW_BATTERY, OUTPUT);
    edu_setup();
      
    Serial.begin(9600);
    while (!Serial) continue;
}

void loop() {
    receiveRequestCommand();
    
    if (millis() - startMillis >= period){
        startMillis = millis();
        sendRobotOdometry();
    }
}

void sendRobotOdometry(){
    //Read Encoders data
    long int time_atu = millis();
    dt = time_atu-time_prev;
    time_prev = time_atu;
    leftCounterPrevious = leftCounter;
    rightCounterPrevious = rightCounter;
    get_counts(&leftCounter, &rightCounter);

    //calculate x, y and theta
    calculateOdometry();
    readBattVoltage();
    
    //Format data
    dataSend = String(dt)          + "," +
               String(leftCounter) + "," +
               String(rightCounter)+ "," +
               String(x)           + "," +
               String(y)           + "," +
               String(theta)       + "," +
               String(batCell0)    + "," +
               String(batCell1)    + "," +
               String(batCell2);
                      
    
    //Send Encoders Data to Esp8266 -> data = dt,encoderLeft,encoderRigth*
    Serial.println(dataSend);
}

void receiveRequestCommand(){
    while(Serial.available()){
      
        //read data from ESP8266
        if(Serial.available() > 0){
            String data = Serial.readStringUntil('\n');
            decodeCommand(data);
        }
    }        
}


void decodeCommand(String message){
    char commandChar = message.charAt(0);

    double velocity, angle;

    switch(commandChar){
        case 'v':
            velocity = message.substring(1).toDouble();
            edu_moveLine(velocity*EDU_VMAX);
            break;
        
        case 'r':
            angle = message.substring(1).toDouble();
            edu_rotate(angle);          
            break;
        
        case 'b':
            edu_stop();        
            break;
        
        case 'n':
            //Commnad neutral
            edu_stop();
            digitalWrite(RIGHT_BRIDGE_A, LOW);
            digitalWrite(RIGHT_BRIDGE_B, LOW);
            digitalWrite(LEFT_BRIDGE_A, LOW);
            digitalWrite(LEFT_BRIDGE_B, LOW);
            break;

        case 'w':
            int iSep = message.indexOf(';', 1);
            if (iSep > 1)
            {
                double left  = message.substring(1, iSep).toDouble();
                double right = message.substring(iSep + 1).toDouble();
                edu_wheel(left, right);
            }
            break;
        
        default:;
    }
}

void calculateOdometry(){

    double dLeft = (leftCounter - leftCounterPrevious) * metersPerPulse;
    double dRight = (rightCounter - rightCounterPrevious) * metersPerPulse; 
    double dCenter = (dLeft+dRight)/2.0;    
    double deltaTheta = ((dRight-dLeft)/dBaseline)*(180.0/PI); 
   
      x   =   x   + dCenter * cos(theta*PI/180.0);
      y   =   y   + dCenter * sin(theta*PI/180.0);
    theta = theta +       deltaTheta;
    if(theta > 360.0){
      theta = theta - 360.0;
    }
    if(theta < 0.0){
      theta = theta + 360.0;
    }
}

double mapd(int x, double in_min, double in_max, double out_min, double out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void readBattVoltage(){
  batCell0 = mapd(analogRead(A2),0.0,1023.0,0.0,5.0);
  batCell0 = batCell0 + batCell0/divisor;
    
  batCell1 = mapd(analogRead(A1),0.0,1023.0,0.0,5.0);
  batCell1 = batCell1 + batCell1/divisor - batCell0;  
  
  batCell2 = mapd(analogRead(A0),0.0,1023.0,0.0,5.0);
  batCell2 = batCell2 + batCell2/divisor - batCell1 - batCell0;

  if((batCell0 < 3.00) || (batCell1 < 3.00) || (batCell2 < 3.00)){
    if (millis() - startMillisBattery >= periodBattery){
        startMillisBattery = millis();
        state = !state;
        digitalWrite(LOW_BATTERY, state);
        digitalWrite(LED_BUILTIN, state);
    }
  }else{
      digitalWrite(LOW_BATTERY, LOW);
      digitalWrite(LED_BUILTIN, LOW);
  }
}
