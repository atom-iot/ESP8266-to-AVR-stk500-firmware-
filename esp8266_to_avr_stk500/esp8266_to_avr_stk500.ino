#include <ESP8266WiFi.h>
#include "WebServ.h"

#define RESET_PIN 2

IPAddress apIP(192, 168, 4, 1);

String atom_ssid = "ARDIFF"; // логин роутера 
String atom_pass = "1234567890"; // пароль роутера
String atom_ssid_AP = "ASB4_1000001"; // SSID AP точки доступа 
String atom_pass_AP = ""; // пароль точки доступа

WiFiServer server(80);

WebServ webServ(RESET_PIN);

void setup()
{
  WiFi.begin(atom_ssid.c_str(), atom_pass.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  
  //WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  //WiFi.softAP(atom_ssid_AP.c_str(), atom_pass_AP.c_str());
  
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  
  server.begin();
}

void loop() {
  
  WiFiClient client = server.available(); 
  
  if (client) {
    String line = client.readStringUntil('\r');
    int httpCmd = webServ.GetCommand(line);
    String urlParam = webServ.GetUrlParam(line);
    
    if(httpCmd == webServ.httpCmdIndex) {
      webServ.WSCmdIndex(&client);
    } else if(httpCmd == webServ.httpCmdDelete) {
      webServ.WSCmdDelete(&client, urlParam);
    } else if(httpCmd == webServ.httpCmdFlash) {
      webServ.WSCmdFlash(&client, urlParam);
    } else if(httpCmd == webServ.httpCmdList) {
      webServ.WSCmdList(&client);
    } else if(httpCmd == webServ.httpCmdUpload) {
      webServ.WSCmdUpload(&client, urlParam);
    } else {
      webServ.WSCmdIndex(&client);
    }
    
    delay(1); 
    client.stop();
  }
}

void formatDevice() {

  SPIFFS.begin();
  SPIFFS.format();
  SPIFFS.end();
  
}














