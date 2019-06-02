#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String ssid = "Finger weg da";
String password = "24680fJhNsdawelobxYA0987AsDfGhBFE";

int redPin = 14; //TX
int bluePin = 3; //RX
int greenPin = 15; //D8
int audioPin = 4; //D2

int r0 = 232;
int g0 = 28;
int b0 = 255;

int r = 0;
int g = 0;
int b = 0;

String modus = "blink";
double fade = 0.2;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WiFi.SSID(), WiFi.psk());
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  server.on("/text", handleRequest);
  server.begin();
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.println(WiFi.localIP());
  Serial.println("Start Loop...");
}
void loop() {
  server.handleClient();
  if (modus == "blink") {
    if (digitalRead(audioPin) == 1) {
      r = r0;
      g = g0;
      b = b0;
      setColor(r, g, b); // Red Color
    } else {
      g = g * fade;
      b = b * fade;
      r = r * fade;
      setColor(r, g, b); // Blue Color
      delay(20);
    }
  } else {
    setColor(r0, g0, b0);
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void handleRequest() {
  Serial.print("Start sending");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(200, "application/json", "{\"message\": \"Set color\"}");

  r0 = server.arg(0).toInt();
  g0 = server.arg(1).toInt();
  b0 = server.arg(2).toInt();
  modus = server.arg(3);
  fade = server.arg(4).toFloat();
}
