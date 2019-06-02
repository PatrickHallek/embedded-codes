int output_value ;

void setup() {

   Serial.begin(9600);

   Serial.println("Reading From the Sensor ...");

   delay(2000);

   }

void loop() {

   output_value= analogRead(A0);

   Serial.print("Mositure : ");
   Serial.print(output_value);
   Serial.print("\n");

   delay(50);

   }
