#include <ESP8266WebServer.h>
#include <Scheduler.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>


#include <EEPROM.h>
#include <sstream>
#include <string>
#include <vector>
#include "Config.h"

SoftwareSerial portOne(14, 16);//(RX, TX) Communication with UNO->SonarsAndBumpers
SoftwareSerial portTwo(12, 13);//(RX, TX) Communication with UNO->VenturinoDrive

double   sonar[7]    = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
bool   bumper[4]    = {0,0,0,0};
long int dt           = 0;
long int leftCounter  = 0;
long int rightCounter = 0;
double   pose[3]    = {0.0,0.0,0.0};
double   batCellVoltage[3]    = {0.0,0.0,0.0};


/*---------------OTHER Functions ----------*/
std::vector<std::string> splitString(const std::string& s, char delimiter){
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}

bool toBool(std::string s) {
  return s == "1.0";
}

/*---------------Task Receive Sensors and Bumpers Data----------*/

class sonarsAndBumperDataTask : public Task {
private:
    String dataSonarsAndBumpers = ""; //dataSonarsAndBumpers = s0,s1,s2,s3,s4,s5,s6,b0,b1,b2,b3
public:
    void setup() {
      
    }
    void loop() {
      //read data from UNO->sonarsAndBumpers
      if (portOne.available() > 0){
          dataSonarsAndBumpers = portOne.readStringUntil('\n');
          int str_len = dataSonarsAndBumpers.length()-1;
          dataSonarsAndBumpers.remove(str_len, 2);

          //slice message
          std::string message = dataSonarsAndBumpers.c_str();
          std::vector<std::string> strArray = splitString(message, ',');

          if(strArray.size() == 11){

            //sonars
            for (int i = 0; i < 7; i++) {
              String s = strArray.at(i).c_str();
              sonar[i] = s.toDouble();
            }
      
            //bumpers
            for (int i = 7; i < 11; i++) {
              bumper[i-7] = ! toBool(strArray.at(i));
            }
            
          }
      }
      delay(10);        
    }
} sonarsAndBumperData_task;

/*---------------Task Receive Encoders Data----------*/

class venturinoDriveTask : public Task {
private:
    String dataEncoders = ""; //dataEncoders = dt,encoderLeft,encoderRigth,x,y,theta
public:
    void setup() {
               
    }
    void loop() {
        //read data from UNO->VenturinoDrive
        if (portTwo.available() > 0){
            dataEncoders = portTwo.readStringUntil('\n');
            int str_len = dataEncoders.length()-1;
            dataEncoders.remove(str_len, 2);

            //slice message
            std::string message = dataEncoders.c_str();
            std::vector<std::string> strArray = splitString(message, ',');

            if(strArray.size() == 9){
                dt = String(strArray.at(0).c_str()).toInt();
                leftCounter = String(strArray.at(1).c_str()).toInt();
                rightCounter = String(strArray.at(2).c_str()).toInt();
                //pose
                for (int i = 3; i < 6; i++) {
                    pose[i-3] = String(strArray.at(i).c_str()).toDouble();
                }
                //batt cell voltage
                for (int i = 6; i < 9; i++) {
                    batCellVoltage[i-6] = String(strArray.at(i).c_str()).toDouble();
                }              
            }
        }       
        delay(10);
    }
} venturinoDrive_task;


/*---------------Communication Tasks-----------------*/

const char* deviceName = "edubot01";
//server ip
const int port = 7000;

WiFiServer wifiServer(port);

String getRobotDataToSend(){
    //dataRobotToWifi = s0,s1,s2,s3,s4,s5,s6,b0,b1,b2,b3,dt,encoderLeft,encoderRigth, x, y, theta
    //dataRobotToWifi = 0.0,0.0,0.0,0.0,0.0,0.0,0.0,false,false,false,false,0,0,0,0.0,0.0,0.0 
  
    String dataStr = String(sonar[0])   + "," +
              String(sonar[1])          + "," +
              String(sonar[2])          + "," +
              String(sonar[3])          + "," +
              String(sonar[4])          + "," +
              String(sonar[5])          + "," +
              String(sonar[6])          + "," +
              String(bumper[0])         + "," +
              String(bumper[1])         + "," +
              String(bumper[2])         + "," +
              String(bumper[3])         + "," +
              String(dt)                + "," +
              String(leftCounter)       + "," +
              String(rightCounter)      + "," +
              String(pose[0])           + "," +
              String(pose[1])           + "," +
              String(pose[2])           + "," +
              String(batCellVoltage[0]) + "," +
              String(batCellVoltage[1]) + "," +
              String(batCellVoltage[2]);
              
    return dataStr;
}

class communicationTask : public Task {
protected:
void loop() {
 
  WiFiClient client = wifiServer.available();
 
  if (client) {
 
    while (client.connected()) {

      //read data from wifi
      if(client.available() > 0){
        String data = client.readStringUntil('\n');
        
        //send data from wifi to robot
        portTwo.println(data);
      }
      //send data from robot to wifi
      client.println(getRobotDataToSend());

      delay(100);
    }
 
    client.stop();

    //robo para quando a conexao cai
    //send data from wifi to robot
    portTwo.println("n");
     
  }
  
}
} communication_task;

/*---------------ESP Setup and loop-----------------*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.begin(1000);
  
  loadConfig();
  int state = atoi(robotConfig.state);
   
  //Static IP address configuration
  IPAddress staticIP(atoi(robotConfig.ip1), atoi(robotConfig.ip2), atoi(robotConfig.ip3), atoi(robotConfig.ip4));                      //ESP static ip
  IPAddress gateway(atoi(robotConfig.gateway1),  atoi(robotConfig.gateway2),  atoi(robotConfig.gateway3), atoi(robotConfig.gateway4)); //IP Address of your WiFi Router (Gateway)
  IPAddress subnet(atoi(robotConfig.subnet1), atoi(robotConfig.subnet2), atoi(robotConfig.subnet3), atoi(robotConfig.subnet4));        //Subnet mask
  IPAddress dns(8, 8, 8, 8);  //DNS
                
  Serial.begin(115200);
  Serial.println("");
  Serial.print("state: ");
  Serial.println(state);
  
  portOne.begin(9600);
  portTwo.begin(9600);  

  int i = 0;
  while(i <= 50){
    if(!digitalRead(bSetup)){
      changeState("1");
      loadConfig();
      state = atoi(robotConfig.state);
      break;
    }
    i++;
    digitalWrite(LED_BUILTIN, HIGH);
    delay(60);
    digitalWrite(LED_BUILTIN, LOW);
    delay(40);    
  }
  if(state == 1){
    WiFi.softAP(ssid0, password0);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print(F("AP IP address: "));
    Serial.println(myIP);
          
    server.on("/", handleRoot);      
    server.on("/config", HTTP_GET, configYourNetwork);
    server.on("/status", HTTP_POST, configYourNetworkSubmit);
    
    server.begin();
  }
  else if(state == 2){
     
      //Wifi config
      WiFi.hostname(deviceName);      // DHCP Hostname (useful for finding device for static lease)
      Serial.print("robot_ip: ");
      Serial.print(robotConfig.ip1);
      Serial.print(",");
      Serial.print(robotConfig.ip2);
      Serial.print(",");
      Serial.print(robotConfig.ip3);
      Serial.print(",");
      Serial.println(robotConfig.ip4);
      Serial.print("   gateway: ");
      Serial.print(robotConfig.gateway1);
      Serial.print(",");
      Serial.print(robotConfig.gateway2);
      Serial.print(",");
      Serial.print(robotConfig.gateway3);
      Serial.print(",");
      Serial.println(robotConfig.gateway4);
      Serial.print("   subnet: ");
      Serial.print(robotConfig.subnet1);
      Serial.print(",");
      Serial.print(robotConfig.subnet2);
      Serial.print(",");
      Serial.print(robotConfig.subnet3);
      Serial.print(",");
      Serial.println(robotConfig.subnet4);     
      
      WiFi.config(staticIP, subnet, gateway, dns);
      
      Serial.print(F("ssid: "));
      Serial.print(robotConfig.ssid);
      Serial.print(F("   password: "));
      Serial.println(robotConfig.password);
      
      WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
      delay(1000);
      WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot      
      
      WiFi.begin(robotConfig.ssid, robotConfig.password);
    
      while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        Serial.println("Connecting..");
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
      }
      Serial.print("Connected to WiFi IP:");
      Serial.println(WiFi.localIP());
     
      wifiServer.begin();
    
      //Tasks
      Scheduler.start(&sonarsAndBumperData_task);
      Scheduler.start(&venturinoDrive_task);
      Scheduler.start(&communication_task);
      Scheduler.begin();
  }
}
 
void loop() {

    int state = atoi(robotConfig.state);
    
    switch (state) {
    case 1:
        server.handleClient();
    break;
    case 2:
        while (WiFi.status() != WL_CONNECTED) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
            Serial.println("Connecting..");
            digitalWrite(LED_BUILTIN, LOW);
            delay(500);
        }    
    break;
    default:
        changeState("1");
        delay(100);
        ESP.restart();
        break;
    }   
  
  }
