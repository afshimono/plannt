#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <user_interface.h>

/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - Arduino UNO or similar microcontroller board
   - Decide how to connect Arduino to the Internet
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* This a code for testing the Wemos D1 as a Web Server. Some debugging information about possible connection error and status are included. */
char auth[] = "0cc1affb75c94f408ef90279afd76dc5";
const char ssid[] = "cOFFee";
const char password[] = "facaseupedido";

/* The following is a code for testing the Wemos D1 as a Web Server. Some debugging information about possible connection error and status are included. */
ESP8266WebServer server(80);

char* readResult(int resultCode){
  if (resultCode == WL_CONNECTED){
    return "Status: Connected.";
  }else if (resultCode == WL_NO_SHIELD){
    return "Status: No Shield.";
  }else if (resultCode == WL_IDLE_STATUS){
    return "Status: Idle.";
  }else if (resultCode == WL_NO_SSID_AVAIL){
    return "Status: SSID not found.";
  }else if (resultCode == WL_SCAN_COMPLETED){
    return "Status: Scan Completed.";
  }else if (resultCode == WL_CONNECT_FAILED){
    return "Status: Failed to Connect.";
  }else if (resultCode == WL_CONNECTION_LOST){
    return "Status: Lost Connection.";
  }else if (resultCode == WL_DISCONNECTED){
    return "Status: Disconnected.";
  }
  else{
    return "Result Unkown.";
  }
  
}
const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  Serial.begin(9600);
  WiFi.persistent(false);
  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
 
  Serial.println("");
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(100);
  
  WiFi.enableSTA(true);
  WiFi.mode(WIFI_STA);
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  //WiFi.beginSmartConfig();
  WiFi.setAutoReconnect(true);
  WiFi.disconnect(false);
  
  WiFi.begin(ssid,password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.println(readResult(WiFi.status()));
    Serial.print("Channel:  ");
    Serial.println(WiFi.channel());
    Serial.print("Wifi Mode:   ");
    if (WiFi.getPhyMode() == WIFI_PHY_MODE_11G){
      Serial.println("11 g");
    } else if (WiFi.getPhyMode() == WIFI_PHY_MODE_11N){
      Serial.println("11 n");
    }else{
      Serial.println("11 b");
    }
    Serial.print("Autoconnect mode:   ");
    Serial.println(WiFi.getAutoConnect());
    Serial.println("");
    delay(3000);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  delay(10);
}


