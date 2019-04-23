#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <RCSwitch.h>
#include <FS.h>
#include <ArduinoJson.h>



String ssid = "placeholder";
String password = "placeholder";
String token = "placeholder";
int connection_status = 2; // 2 - default, 0 - connection failed, 1 - connected
int server_timeout = 120000;
int soil = 0;

const char* host = "192.168.188.22";
const int httpPort = 3000;

IPAddress apIP(192, 168, 169, 69);
ESP8266WebServer server(80);
RCSwitch rcSwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");
  rcSwitch.enableReceive(D1);
 // WiFi.begin(WiFi.SSID(), WiFi.psk());
  int timer = millis();
  Serial.println("Check WifiCredentials");
 // while (WiFi.status() != WL_CONNECTED) {
 //   Serial.print(".");
 //   if ((millis() - timer) > 15000) {
 //     break;
 //   }
 //   delay(1000);
 // }
  // if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Start WifiConfiguration");
    wifiService();
    WiFi.mode(WIFI_OFF);
    WiFi.begin(WiFi.SSID(), WiFi.psk());
   //};
  loadConfig();
  if (token == "placeholder") {
    Serial.println("Couldn't load conifg data");
    Serial.println("resetting");
    ESP.reset();
  }
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
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("WaterMonitor", "admin1234");
  server.on("/text", handleRequest);
  server.on("/status", statusRequest);
  server.begin();
  Serial.println("HTTP server started");
  int time_saver = millis();
  while (millis() - time_saver < server_timeout) {
    server.handleClient();
    delay(0);
  }
  Serial.println("\n After waiting ...");
}

void statusRequest() {
  Serial.print("Request status: \n");
  Serial.println(connection_status);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(200, "application/json", "{\"message\": \"" + (String)connection_status + "\"}");
  if (connection_status == 1) {
    delay(10000);
    server_timeout = 0;
  }
}

void handleRequest() {
  Serial.print("Start sending");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(200, "application/json", "{\"message\": \"Credentials received\"}");
  String message = "";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": " + server.arg(i) + "\n";
  }
  token = server.arg(0);
  ssid = server.arg(1);
  password = server.arg(2);
  int timer = millis();
  Serial.print("Start Connecting \n");
  Serial.print(ssid);
  Serial.print("\n");
  Serial.print(password);
  Serial.print("\n");
  WiFi.begin(ssid, password);
  delay(1000);

  Serial.println(WiFi.status());
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if ((millis() - timer) > 20000) {
      break;
    }
    delay(1000);
  }
  Serial.println(WiFi.status());
  if (WiFi.status() == WL_CONNECTED) {
    connection_status = 1;
    Serial.println("\n Successfully connected! \n");
    saveConfig();
  } else {
    Serial.println("Wrong password");
    connection_status = 0;
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
