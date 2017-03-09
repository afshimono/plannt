#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <user_interface.h>

/* This a code for testing the Wemos D1 as a Web Server. Some debugging information about possible connection error and status are included. */

const char ssid[] = "cOFFee";
const char password[] = "facaseupedido";

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


