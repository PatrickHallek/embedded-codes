#include <Servo.h>
Servo servo; // Define our Servo
int pos = 0;
int offset =0;
int movement_angle = 15;
int movement_speed = 7;

void setup()
{
  Serial.begin(9600);
  servo.attach(9);
}

void loop()
{
  int ran = random(5000);
  Serial.print(ran);
  Serial.print("\n");
  if(ran == 3){
    movement_speed++;
  }
  if(ran == 4){
    movement_speed--;
  }
  if(ran == 2){
    delay(3 * 60 * 1000);
  }
  rotate_forewards(movement_speed, movement_angle);
  delay(100);
  rotate_backwards(movement_speed+5, movement_angle);
}

void rotate_forewards(int delay_time,int angle){
  while (pos <=angle+offset) {
    servo.write(pos);
    delay(delay_time);
    pos++;
  }
}

void rotate_backwards(int delay_time,int angle){
  while (pos >=0+offset) {
    servo.write(pos);
    delay(delay_time);
    pos--;
  }
}
