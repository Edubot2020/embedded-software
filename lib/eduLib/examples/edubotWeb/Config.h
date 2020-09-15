#ifndef __CONFIG_H__
#define __CONFIG_H__
    
    //-----------------------------------------------
    //Parâmetros de configuração do EDUBOT
    //-----------------------------------------------
    Sonar sonarRight(TRIGR, ECHOR);
    Sonar sonarFront(TRIGF, ECHOF);
    Sonar sonarLeft(TRIGL, ECHOL);    
    long distR, distF,distL;    
    
    struct ConfigStruct
    {
      char ssid[50];
      char password[50];
      char state[50];
           
    } edubotConfig;

    //-----------------------------------------------    
    //Parametros do api web
    //-----------------------------------------------
    //String URL = "http://www.mysmarthome.com.br/webadmin/api/v1/module"; //Specify request destination
    String URL = "http://192.168.1.152/projects/inProgress/home-automation/www/webadmin/api/v1/module";
    String contentType = "application/x-www-form-urlencoded";
    int apiConnectionTimeout = 60000; // aguarda *ms to end connection
    
    //-----------------------------------------------
    //Functions
    //-----------------------------------------------
    boolean send_Data(String requestType);
    void medeDistancias();
    void mandaDistanciasSerial();
    void colisao();
#endif
