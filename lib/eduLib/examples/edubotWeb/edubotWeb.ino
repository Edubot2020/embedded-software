#include <Edubot.h>
#include <LibMotor.h>
#include <Encoder.h>
#include <LibSonar.h>
#include <Controller.h>
#include <ArduinoJson.h>
#include "Config.h"

void setup() {
    // Inicializa a serial
    Serial.begin(115200);

    edu_setup();

    //WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    //WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
    
    Serial.print("SSID: ");
    Serial.println(edubotConfig.ssid);
    Serial.print("Password: ");
    Serial.println(edubotConfig.password);
        
    //WiFi.begin(edubotConfig.ssid, edubotConfig.password);     //Connect to your WiFi router
    Serial.println("");
    
    Serial.print("Connecting");
    // Wait for connection
    //while (WiFi.status() != WL_CONNECTED) {
      //digitalWrite(ledWifi, HIGH);
      delay(250);
      Serial.print(".");
      //digitalWrite(ledWifi, LOW);
      delay(250);
    //}
    //digitalWrite(ledWifi, HIGH);  
    
    //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(edubotConfig.ssid);
    Serial.print("IP address: ");
    //Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void loop() {
    medeDistancias();
    send_Data("updateRobo");  
}
