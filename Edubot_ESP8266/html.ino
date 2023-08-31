const char style_html[] = "<!DOCTYPE html><html lang=\"pt-br\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Edubot 2020</title><style>*{margin: 0;padding: 0;}body {background-color: #eeeeee;font-family: Arial, Helvetica, Sans-Serif;text-align: center;}h1{color: #272728;width: 100%;font-size: 24px;padding-top: 20px;padding-bottom: 20px;}h2{color: #494949;width: 100%;font-size: 18px;padding-top: 10px;padding-bottom: 10px;}h3{color: #494949;width: 100%;font-size: 14px;padding-top: 10px;padding-bottom: 10px;font-weight: normal;}hr{color: #494949;}.formConfig{width: 96%;padding-left: 2%;display: table;}.formConfig p{text-align: left;width: 100%;display: table;color: #494949;font-weight: bold;font-size: 18px;padding-top: 30px;padding-bottom: 10px;}.formConfig label{text-align: left;width: 100%;display: table;color: #494949;font-size: 18px;padding-top: 10px;padding-bottom: 10px;}.formConfig input{text-align: left;width: 90%;display: table;color: #494949;font-size: 16px;padding: 2%;padding-top: 10px;padding-bottom: 10px;max-width: 500px;}.formConfig input[type=\"submit\"]{color: #0a0a0a;background: #71dd74;text-align: center;cursor:pointer;border:none;border-radius: 4px;margin-top: 20px;margin-bottom: 40px;width: 250px;}.initConfig{margin-top: 40px;}.initConfig a{padding: 20px;font-size: 18px;color: #0a0a0a;background: #71dd74;text-decoration: none;border-radius: 4px;cursor:pointer;}.formConfig .input-group{width: 90%;max-width: 500px;display: table;}.formConfig .input-group input{width: 100%;max-width: 18%;float: left;margin-right:1%;text-align:center;}</style></head>";
      
void handleRoot(){
    const char index_html[]  = "<body><h1>Edubot 2020</h1><hr><h2>Assistente de configuração da Rede Wifi</h2><hr><p class=\"initConfig\"><a href=\"config\">Iniciar configuração</a></p></body></html>";
    String html = String(style_html) + String(index_html);    
    server.send(200, "text/html", html);
}

void configYourNetwork(){
    const char config_html[] = "<body><h1>Edubot 2020</h1><hr><h2>Configuração da rede</h2><hr><div class=\"formConfig\"><form method=\"POST\" action=\"status\"><p>Acesso a rede Wifi</p><label>Nome da sua rede WIFI (SSID):</label><input type=\"text\" name=\"ssid-Wifi\" value=\"\"/><label>Senha da sua rede WIFI (password):</label><input type=\"text\" name=\"password-Wifi\" value=\"\" /><label>IP Estático:</label><div class=\"input-group\"><input type=\"number\" name=\"ip1\" value=\"192\" /><input type=\"number\" name=\"ip2\" value=\"168\" /><input type=\"number\" name=\"ip3\" value=\"0\" /><input type=\"number\" name=\"ip4\" value=\"100\" /></div><label>Gateway:</label><div class=\"input-group\"><input type=\"number\" name=\"gateway1\" value=\"192\" /><input type=\"number\" name=\"gateway2\" value=\"168\" /><input type=\"number\" name=\"gateway3\" value=\"0\" /><input type=\"number\" name=\"gateway4\" value=\"1\" /></div><label>Máscara (subnet):</label><div class=\"input-group\"><input type=\"number\" name=\"subnet1\" value=\"255\" /><input type=\"number\" name=\"subnet2\" value=\"255\" /><input type=\"number\" name=\"subnet3\" value=\"255\" /><input type=\"number\" name=\"subnet4\" value=\"0\" /></div><p>Após salvar o robô irá reiniciar em 10 segundos.</p><input type=\"submit\" name=\"botao\" value=\"Salvar\" /></form></div></body></html>";
    String html = String(style_html) + String(config_html);    
    server.send(200, "text/html", html);
}

void validateNumber(String data, bool &status){
  int number = data.toInt();
  if(number < 0 || number > 255) status = false;
}

void configYourNetworkSubmit(){
    bool dataOk = true;
    String ssidWifi = server.arg("ssid-Wifi");
    String passwordWifi = server.arg("password-Wifi");
    String ip1 = server.arg("ip1");
    validateNumber(ip1, dataOk);
    String ip2 = server.arg("ip2");
    validateNumber(ip2, dataOk);
    String ip3 = server.arg("ip3");
    validateNumber(ip3, dataOk);
    String ip4 = server.arg("ip4");
    validateNumber(ip4, dataOk);
    String gateway1 = server.arg("gateway1");
    validateNumber(gateway1, dataOk);
    String gateway2 = server.arg("gateway2");
    validateNumber(gateway2, dataOk);
    String gateway3 = server.arg("gateway3");
    validateNumber(gateway3, dataOk);
    String gateway4 = server.arg("gateway4");
    validateNumber(gateway4, dataOk);
    String subnet1 = server.arg("subnet1");
    validateNumber(subnet1, dataOk);
    String subnet2 = server.arg("subnet2");
    validateNumber(subnet2, dataOk);
    String subnet3 = server.arg("subnet3");
    validateNumber(subnet3, dataOk);
    String subnet4 = server.arg("subnet4");
    validateNumber(subnet4, dataOk);

    if(dataOk){
      ssidWifi.toCharArray(robotConfig.ssid, 50);
      passwordWifi.toCharArray(robotConfig.password, 50);
      ip1.toCharArray(robotConfig.ip1, 5);
      ip2.toCharArray(robotConfig.ip2, 5);
      ip3.toCharArray(robotConfig.ip3, 5);
      ip4.toCharArray(robotConfig.ip4, 5);
      gateway1.toCharArray(robotConfig.gateway1, 5);
      gateway2.toCharArray(robotConfig.gateway2, 5);
      gateway3.toCharArray(robotConfig.gateway3, 5);
      gateway4.toCharArray(robotConfig.gateway4, 5);
      subnet1.toCharArray(robotConfig.subnet1, 5);
      subnet2.toCharArray(robotConfig.subnet2, 5);
      subnet3.toCharArray(robotConfig.subnet3, 5);
      subnet4.toCharArray(robotConfig.subnet4, 5);
      
      const char submit_html[] = "<body><h1>Edubot 2020</h1><hr><h2>Configuração realizada com sucesso!</h2><hr><h3>Confira abaixo um resumo das configurações</h3><hr><div class=\"formConfig\"><form method=\"POST\"><h2>Restart do robô em 10 segundos!</h2><label>Nome da sua rede WIFI (SSID):</label><input type=\"text\" name=\"ssid-Wifi\" value=\"";
      String partOne   = "\" readonly><label>Senha da sua rede WIFI (password):</label><input type=\"text\" value=\"";                               
      String partTwo   = "\" readonly><label>IP Estático:</label><input type=\"text\" value=\"";
      String partThree = "\" readonly><label>Gateway:</label><input type=\"text\" value=\"";          
      String partFour  = "\" readonly><label>Máscara (subnet):</label><input type=\"text\" value=\"";   
      String partFive  = "\" readonly></form></div></body></html>";             
      
      String html = String(style_html) + String(submit_html) + ssidWifi + partOne + passwordWifi + partTwo + ip1 + "." + ip2 + "." + ip3 + "." + ip4 + partThree + gateway1 + "." + gateway2 + "." + gateway3 + "." + gateway4 + partFour + subnet1 + "." + subnet2 + "." + subnet3 + "." + subnet4 + partFive;    
       
      //String html = "Aguarde o RESTART do robô em 10 segundos!!!";
      server.send(200, "text/html", html);
      
      saveConfig();
      changeState("2");
      delay(10000);
      ESP.restart();
    }
    else{
        const char index_html[]  = "<body><h1>Edubot 2020</h1><hr><h2>Parâmetros de conexão estão inválidos!</h2><hr><p class=\"initConfig\"><a href=\"config\">Voltar para a configuração</a></p></body></html>";
        String html = String(style_html) + String(index_html);  
        server.send(200, "text/html", html);
    }
}
