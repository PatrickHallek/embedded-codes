int sensor_pin = 10;
int out_pin = 9;

int output_value ;

void setup() {

   Serial.begin(9600);

   Serial.println("Reading From the Sensor ...");

   delay(2000);

   }

void loop() {

   output_value= digitalRead(sensor_pin);
   Serial.print(output_value);
   analogWrite(out_pin, output_value);

   Serial.print("Mositure : ");
   Serial.print(output_value);
   Serial.print("\n");

   delay(2);

   }
