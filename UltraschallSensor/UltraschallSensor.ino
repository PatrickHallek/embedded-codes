#include <NewPing.h>

#define TRIG_PIN 10
#define ECHO_PIN 11
#define MAX_DIST 255

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DIST);

int redPin = 4;
int greenPin = 3;
int bluePin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  unsigned int distance = sonar.ping_cm();
  Serial.print(distance);
  Serial.println("cm");
  distance = 255 - distance;
  analogWrite(A0, distance);
  delay(20);
}
