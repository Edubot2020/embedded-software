

#include <LibSonar.h>

double dataSonar[7]  = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};     //dataSonar[14] = {s0,s1,s2,s3,24,s5,s6}
int dataBumper[4] = {0,0,0,0};                            //dataBumper[4] = {b0,b1,b2,b3}

int firstReadBumper[4] = {0,0,0,0};
boolean flagBumper = false;
unsigned long startMillisBumper = 0;
const unsigned long periodBumper = 300;

unsigned long startMillis = 0;
const unsigned long period = 100;

//pinos dos sonares
#define ECHO0  2  //Echo Sonar 0
#define TRIG0  3  //Trig Sonar 0
#define ECHO1  5  //Echo Sonar 1
#define TRIG1  4  //Trig Sonar 1
#define ECHO2 A3  //Echo Sonar 2
#define TRIG2 A4  //Trig Sonar 2
#define ECHO3 A1  //Echo Sonar 3
#define TRIG3 A2  //Trig Sonar 3
#define ECHO4 13  //Echo Sonar 4
#define TRIG4 A0  //Trig Sonar 4
#define ECHO5 11  //Echo Sonar 5
#define TRIG5 12  //Trig Sonar 5
#define ECHO6  8  //Echo Sonar 6
#define TRIG6  9  //Trig Sonar 6
#define BUMPER_FL A5 //Bumper front/left
#define BUMPER_FR 10 //Bumper front/right
#define BUMPER_BL  7 //Bumper back/left
#define BUMPER_BR  6 //Bumper back/right

//criando objetos da classe sonar
Sonar sonar0(TRIG0,ECHO0);
Sonar sonar1(TRIG1,ECHO1);
Sonar sonar2(TRIG2,ECHO2);
Sonar sonar3(TRIG3,ECHO3);
Sonar sonar4(TRIG4,ECHO4);
Sonar sonar5(TRIG5,ECHO5);
Sonar sonar6(TRIG6,ECHO6);
// o tempo de time out default é de 3000us (equivalente a 5m)

void setup() {
    pinMode(BUMPER_FL, INPUT_PULLUP);
    pinMode(BUMPER_FR, INPUT_PULLUP);
    pinMode(BUMPER_BL, INPUT_PULLUP);
    pinMode(BUMPER_BR, INPUT_PULLUP);

    Serial.begin(9600);
    while (!Serial) continue;
}

void loop() {
    if (millis() - startMillis >= period){
        startMillis = millis();

        //Read Sensors
        double readSonar[7]  = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};        
        readSonar[0]  = sonar0.measureDist();
        readSonar[1]  = sonar1.measureDist();
        readSonar[2]  = sonar2.measureDist();
        readSonar[3]  = sonar3.measureDist();
        readSonar[4]  = sonar4.measureDist();
        readSonar[5]  = sonar5.measureDist();
        readSonar[6]  = sonar6.measureDist();
        for(int i = 0; i < 7; i++){
            if(readSonar[i] < 500.00){
                dataSonar[i] = readSonar[i];
            }
        }

        firstReadBumper[0] = digitalRead(BUMPER_FL);
        firstReadBumper[1] = digitalRead(BUMPER_FR);
        firstReadBumper[2] = digitalRead(BUMPER_BL);
        firstReadBumper[3] = digitalRead(BUMPER_BR);

       if((firstReadBumper[0] == 1 || firstReadBumper[1] == 1 || firstReadBumper[2] == 1 || firstReadBumper[3] == 1) && flagBumper == false){
          flagBumper = true;
          startMillisBumper = millis();
       }else{
          if (millis() - startMillisBumper >= periodBumper){
              flagBumper = false;
              dataBumper[0] = digitalRead(BUMPER_FL);
              dataBumper[1] = digitalRead(BUMPER_FR);
              dataBumper[2] = digitalRead(BUMPER_BL);
              dataBumper[3] = digitalRead(BUMPER_BR);
          }
       }        
    
        //Format data
        String dataSend = String(dataSonar[0], 2) + "," +
                          String(dataSonar[1], 2) + "," +
                          String(dataSonar[2], 2) + "," +
                          String(dataSonar[3], 2) + "," +
                          String(dataSonar[4], 2) + "," +
                          String(dataSonar[5], 2) + "," +
                          String(dataSonar[6], 2) + "," +
                          String(dataBumper[0] == 1? "1.0" : "0.0") + "," +
                          String(dataBumper[1] == 1? "1.0" : "0.0") + "," +
                          String(dataBumper[2] == 1? "1.0" : "0.0") + "," +
                          String(dataBumper[3] == 1? "1.0" : "0.0");
                          
        //Send Read Sensors to Esp8266 -> dataSend = s0,s1,s2,s3,s4,s5,s6,b0,b1,b2,b3
        Serial.println(dataSend);
    }
}
