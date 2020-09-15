/*
 * @example ConnectWiFi.ino
 * @brief The ConnectWiFi demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.03
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <ESP8266.h>
#define SSID        "WAPGCAR"
#define PASSWORD    ""
#define TCPPORT 8090

ESP8266 wifi(Serial1);


void setup(void)
{
    Serial.begin(9600);
    Serial1.begin(115200);
    Serial.print("setup begin\r\n");

    Serial.print("FW Version: ");
    Serial.println(wifi.getVersion().c_str());
    
    // Sets operation in Station mode and AP
    if (wifi.setOprToStationSoftAP()) {
		Serial.print("Station and AP OK\r\n");
    } else {
        Serial.print("Error setting Station and AP\r\n");
    }

    // Connects to wifi point
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");       
        Serial.println(wifi.getLocalIP().c_str());
    } else {
        Serial.print("Join AP failure\r\n");
    }

    // Enables MUX to allow multiple connections
    if (wifi.enableMUX()) {
        Serial.print("MUX ok\r\n");
    }else{
        Serial.print("MUX error\r\n");
    } 

    // Starts TCP server on defined port

    if (wifi.startTCPServer(TCPPORT)) {
        Serial.print("TCP server started\r\n");
    }else{
        Serial.print("Error starting TCP Server\r\n");
    } 

    // setup complete
    Serial.print("setup end\r\n");
}

// Implements echo functionality, so as to test the connection
void loop(void)
{
	uint8_t buffer[128]={0};
	uint8_t mux_id;

	uint8_t len=wifi.recv(&mux_id,buffer,sizeof(buffer),100);
	if(len>0)
	{
		//Connection Status
		Serial.print("Status:[");
		Serial.print(wifi.getIPStatus().c_str());
		Serial.println("]");

		//MUX ID
		Serial.print("ID:");Serial.print(mux_id);
		Serial.print("[");
		for(uint32_t i=0;i<len;i++)
			Serial.print((char)buffer[i]);
		Serial.println("]");

    	if (wifi.send(mux_id,buffer,len)) {
    	    Serial.print("Message sent back\r\n");
    	}else{
    	    Serial.print("Error sending message back\r\n");
    	} 

	//Releases the connection
    	//if (wifi.releaseTCP(mux_id)) {
    	//    Serial.print("Released connection with ID no:");Serial.println(mux_id);
    	//}else{
    	//    Serial.print("error releasing connection with IP no:\r\n");Serial.println(mux_id);
    	//} 
		
		Serial.print("Status:[");
		Serial.print(wifi.getIPStatus().c_str());
		Serial.println("]");

	}
}

