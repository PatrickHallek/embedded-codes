#include <RCSwitch.h>

RCSwitch rcSwitch = RCSwitch(); 

void setup() {
  Serial.begin(9600);
  Serial.println("Start Sending ...");
  rcSwitch.enableTransmit(3);
}
void loop() {
  int soil =  map(analogRead(A2), 0, 1023, 0, 99);
  Serial.println("Soil:");
  Serial.print(soil);
  String key = "99";
  String deviceNumber = "100";
  String code  = (String)key + deviceNumber + (String)soil;
  rcSwitch.send(code.toInt(),24);
  delay(1000);
}
