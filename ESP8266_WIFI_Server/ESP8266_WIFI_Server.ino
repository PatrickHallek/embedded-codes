#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <RCSwitch.h>
#include <FS.h>
#include <ArduinoJson.h>

String ssid = "placeholder";
String password = "placeholder";
String token = "placeholder";
int server_timeout = 60000;
int soil = 0;

const char* host = "192.168.188.22";
const int httpPort = 3000;

IPAddress apIP(192, 168, 188, 81);
ESP8266WebServer server(80);
RCSwitch rcSwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");
  rcSwitch.enableReceive(D1);
  Serial.println("Start WifiConfiguration");
  wifiService();
  WiFi.mode(WIFI_OFF);
  WiFi.begin(WiFi.SSID(), WiFi.psk());
  loadConfig();
}

void loop() {
  if (rcSwitch.available()) {
    soil = rcSwitch.getReceivedValue();
    postRequest(soil);
    delay(5000);
  }
  rcSwitch.resetAvailable();
}

void wifiService() {
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("WaterMonitor", "admin1234");
  server.on("/text", handleRequest);
  server.begin();
  Serial.println("HTTP server started");
  int time_saver = millis();
  while (millis() - time_saver < server_timeout) {
    // dnsServer.processNextRequest();
    server.handleClient();
    delay(0);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    int timer = millis();
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      if ((millis() - timer) > 15000) {
        break;
      }
      delay(1000);
    }
  }
  Serial.println("\n After waiting ...");
}

void handleRequest() {
  String message = "";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.print(message);
  token = server.arg(0);
  ssid = server.arg(1);
  password = server.arg(2);
  int timer = millis();
  Serial.print("Start Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if ((millis() - timer) > 15000) {
      break;
    }
    delay(1000);
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  if (WiFi.status() == WL_CONNECTED) {
    server.send(200, "application/json", "{\"message\": \"Success\"}");
    Serial.print("\n Successfully connected! \n");
    saveConfig();
    server_timeout = 0;
  } else {
    server.send(500, "application/json", "{\"message\": \"Failed\"}");
    Serial.print("Something went wrong.");
  }
}

void postRequest(int soil) {
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String content = "{\"soilMoisture\": " + String(soil) + ",\"appToken\": \"" + String(token) + "\"}";
  client.println("POST / HTTP/1.1");
  client.println("Host: server_name");
  client.println("Accept: */*");
  client.println("Content-Length: " + String(content.length()));
  client.println("Content-Type: application/json");
  client.println();
  client.println(content);
  Serial.println(content);
}

bool loadConfig() {
  if (SPIFFS.begin())
  {
    Serial.println("SPIFFS Initialisierung....OK");
  }
  else
  {
    Serial.println("SPIFFS Initialisierung...Fehler!");
  }

  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  token = (const char*)json["token"];
  Serial.print("\n Loaded token: ");
  Serial.print(token);
  Serial.print("\n");
  return true;
}

bool saveConfig() {
  if (SPIFFS.begin())
  {
    Serial.println("SPIFFS Initialisierung....OK");
  }
  else
  {
    Serial.println("SPIFFS Initialisierung...Fehler!");
  }

  //Format File System
  if (SPIFFS.format())
  {
    Serial.println("Datei-System formatiert");
  }
  else
  {
    Serial.println("Datei-System formatiert Error");
  }
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["token"] = token;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  Serial.print("Successfully stored config");
  return true;
}
