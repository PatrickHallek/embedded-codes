#include <ESP8266WiFi.h>
#include <WiFiManager.h>          
#include <DNSServer.h>
#include <RCSwitch.h>

const char* host = "192.168.188.22";
const int httpPort = 3001;
int soil = 0;
char* user = "Patrick";
RCSwitch rcSwitch = RCSwitch(); 
WiFiClient client;

void setup() {
  Serial.begin(9600);
  Serial.print("Start receiving ...");
  rcSwitch.enableReceive(D2);
  
  WiFiManager wifiManager;  
  Serial.println(WiFi.localIP()); 
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  Serial.println("Opening configuration portal");

  // wifiManager.autoConnect("Water Monitor", "admin");
  if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(30);
  if (!wifiManager.startConfigPortal())
  {
     Serial.println("Not connected to WiFi but continuing anyway.");
  } 
  else 
  {
     Serial.println("connected...yeey :)");
  }

  if (WiFi.status()!=WL_CONNECTED)
  {
      Serial.println("failed to connect, finishing setup anyway");
  } 
  else
  {
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
  }
}

void loop(){
    int soil = 0;
    if (rcSwitch.available()){
      soil = rcSwitch.getReceivedValue();
      if (soil != 0) {
        String content = "{\"soilMoisture\": " + String(soil) + ",\"user\": \"" + String(user) + "\"}";
        client.println("POST / HTTP/1.1");
        client.println("Host: server_name");
        client.println("Accept: */*");
        client.println("Content-Length: " + String(content.length()));
        client.println("Content-Type: application/json");
        client.println();
        client.println(content);
        Serial.println(content);
        rcSwitch.resetAvailable();
        delay(500);
      }
    }
  delay(0);
}
