// This Example code is for use with the Controlling a Standard Servomotor exercise
//

// Include the servo library to allow for connection and control of servo motors
#include <Servo.h>

// define the macro pinServo1 as 9, to use for connecting the servo to pin 9
#define pinServo1 9

int potAnalogPin = 0;
int potReading; 
int mappedReading;
int mappedReading2;
// Declare a servo motor class variable servo 1
Servo servo1;

// -------------------------------------
// Setup function
void setup()
{
  Serial.begin(9600); 
  // "attach"  servo 1 to the defined pin
  servo1.attach(pinServo1);

  // set the neutral, centre, position for the servo
  servo1.write(90);

  // Alternte command for setting the neutral position
  //servo1.writeMicroseconds(1500);

  // wait until the servo has moved into position, (1000ms is more time than needed)
  delay(1000);
}

// -------------------------------------
// Loop Function
void loop()
{
  potReading = analogRead(potAnalogPin);
  mappedReading = map(potReading, 0, 1023, 0, 180);
  Serial.print("Potentiometer Angle measurement = ");
  Serial.print(map(potReading, 0, 1023, -150, 150));
  Serial.print(", Servo command angle = ");
  Serial.print(mappedReading);
  Serial.print(", Actual angle = ");
  Serial.println(mappedReading - 90);
  servo1.write(mappedReading);
  delay(50);
}
