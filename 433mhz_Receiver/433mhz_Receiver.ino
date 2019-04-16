#include <RCSwitch.h>
 
RCSwitch rcSwitch = RCSwitch();
 
void setup() {
  Serial.begin(9600); //Begin der seriellen Kommunikation mit 9600 baud.
  // Empfängermodul ist am digitalen PIN 2 angeschlossen
  // dies ist der Interrupt-Pin "0".
  Serial.print("Start receiving ...");
  rcSwitch.enableReceive(D2);
}
 
void loop() {
  if (rcSwitch.available()){ //Wenn Daten verfügbar sind.
   //Ausgabe des gelesenen Wertes
    int value = rcSwitch.getReceivedValue();
    Serial.println(value);
    
    //Zurücksetzen des Empfängers.
    rcSwitch.resetAvailable();
  }
}
