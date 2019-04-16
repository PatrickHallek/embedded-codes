#include <RCSwitch.h>
 
RCSwitch rcSwitch = RCSwitch(); 
RCSwitch mySwitch = RCSwitch(); 

void setup() {
  Serial.begin(115200);
  // Sendemodul an dem digitalen PIN 9 angeschlossen.
  Serial.print("Start sending ...");
  rcSwitch.enableTransmit(9);
}
void loop() {
  int soil =  map(analogRead(A0), 0, 1023, 0, 99);
  String key = "99";
  String deviceNumber = "100";
  String code  = (String)key + deviceNumber + (String)soil;
  Serial.print(code);
  Serial.print("\n");
  Serial.print("Send data\n");
  rcSwitch.send(code.toInt(),24);
  delay(5000);
}
