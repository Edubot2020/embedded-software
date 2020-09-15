#ifndef __CONFIG_H__
#define __CONFIG_H__

#define FX(string_literal) (reinterpret_cast<const __FlashStringHelper *>(((__extension__({static const char __c[] __attribute__((section(".irom.text.myheader"))) = ((string_literal)); &__c[0];})))))

//-----------------------------------------------
//GPIO
//-----------------------------------------------
int const bSetup = 0;

//-----------------------------------------------
//Parâmetros Web Server
//-----------------------------------------------
#ifndef APSSID
    #define APSSID "Edubot2020"
    #define APPSK  "password"
#endif /* APSSID */
const char *ssid0 = APSSID;
const char *password0 = APPSK;
ESP8266WebServer server(80);


//-----------------------------------------------
//Parâmetros de configuração da EEPROM
//-----------------------------------------------
#define VERSION_START  500
#define CONFIG_START   6
#define CONFIG_VERSION "1a"

struct ConfigStruct{

    char state[5];
    char ssid[50];
    char password[50];
    char ip1[5];
    char ip2[5];
    char ip3[5];
    char ip4[5];
    char gateway1[5];
    char gateway2[5];
    char gateway3[5];
    char gateway4[5];
    char subnet1[5];
    char subnet2[5];
    char subnet3[5];
    char subnet4[5];    

    
} robotConfig;

//-----------------------------------------------
//Functions
//-----------------------------------------------

void handleRoot();
void configYourNetwork();
void configYourNetworkSubmit();
void saveConfig() ;
void loadConfig();

#endif /* __CONFIG_H__ */
