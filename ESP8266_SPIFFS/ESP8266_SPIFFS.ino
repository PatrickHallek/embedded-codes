#include "FS.h" // Hier binden wir die benötigte Bibliothek ein

void setup() {
  Serial.begin(9600); // Serielle Ausgabe aktivieren
  delay(1000); // Kurze Pause, damit wir Zeit haben den Seriellen Monitor zu öffnen.
  
  SPIFFS.begin(); // Filesystem mounten
  File f = SPIFFS.open( "/test.txt", "r"); // Datei zum lesen öffnen
  if (!f) {
    Serial.println("file open failed");
  }
  String data = f.readString(); // Inhalt der Textdatei wird gelesen...
  Serial.println("Inhalt der geöffneten Datei:");
  Serial.println(data); // ... und wieder ausgegeben
  f.close(); // Wir schließen die Datei
}
void loop() {
}
