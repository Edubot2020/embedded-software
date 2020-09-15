
boolean send_Data(String requestType){

    boolean success = false;

    // start tcp connection
    //HTTPClient http; //Declare object of class HTTPClient
    //http.begin(URL); //Specify request destination
    //http.setReuse(true);
    //http.setTimeout(apiConnectionTimeout); // define the api connection timeout
    //http.addHeader("Content-Type", contentType); //Specify content-type header
  
    if(requestType == "updateRobo"){

      String data;

         // send data by post
        //int httpCode = http.POST(data);

        //if (httpCode > 0) {
          //Serial.printf("[HTTP] POST... code: %d\n", httpCode);
          
          // file found at server
          //if (httpCode == HTTP_CODE_OK) {

            // get payload at server
            String payload;// = http.getString();
            //Serial.print("payload update: "); Serial.println(payload);    //Print request response payload

            // decode json object
            DynamicJsonDocument doc(2048);
            deserializeJson(doc, payload);
            JsonObject obj = doc.as<JsonObject>();

            String message = obj["message"];
            
            // process message content
            if(boolean(obj["status"]) == true){
              Serial.print("Mensagem do status em updateRobo==true: ");
              Serial.println(message);

              //edu_moveReto(0.8*EDU_VMAX); //void edu_moveReto(double Speed);
              //edu_para();
              //delay(500);
              //edu_rotaciona(-90); //void edu_rotaciona(double Angulo);
              
              success = true;
            }
            else{
              Serial.print("Mensagem do status em updateRobo==false: ");
              Serial.println(message);
            }
            
          }
          
        //} else {
          //Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());          
        //}

    //}

    // end connection
    //http.end();
    delay(100);
    return success;
}
