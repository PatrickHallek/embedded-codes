
#include <ESP8266WiFi.h>
#include <WiFiManager.h>          
#include <DNSServer.h>


const char* host = "192.168.188.22";
const int httpPort = 3000;

void setup() {
  Serial.begin(115200);
  Serial.println("\n Starting");
  
  Serial.println(WiFi.localIP()); 
 
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  Serial.println("Opening configuration portal");

  WiFiManager wifiManager;  
  if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(1);
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

void loop() {
  int soil= analogRead(A0);
  char* user = "Patrick" ;
  
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
   String content = "{\"soilMoisture\": " + String(soil) + ",\"user\": \"" + String(user) + "\"}";
   Serial.print("Requesting POST: ");
   // Send request to the server:
   client.println("POST / HTTP/1.1");
   client.println("Host: server_name");
   client.println("Accept: */*");
   client.println("Content-Length: " + String(content.length()));
   client.println("Content-Type: application/json");
   client.println();
   client.println(content);
   Serial.println(content);

   delay(500); // Can be changed
  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }
  delay(500);
}
