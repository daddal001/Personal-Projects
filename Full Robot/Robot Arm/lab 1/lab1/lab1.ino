// This Example code is for use with the Controlling a Standard Servomotor exercise
//

// Include the servo library to allow for connection and control of servo motors
#include <Servo.h>

// define the macro pinServo1 as 9, to use for connecting the servo to pin 9
#define pinServo1 11
#define pinServo2 10
#define pinServo3 5

int potAnalogPin1 = 0;
int potAnalogPin2 = 1;
int potAnalogPin3 = 2;
int potReading1; 
int potReading2; 
int potReading3; 
int mappedReading1;
int mappedReading2;
int mappedReading3;
// Declare a servo motor class variable servo 1
Servo servo1;
Servo servo2;
Servo servo3;


// -------------------------------------
// Setup function
void setup()
{
  Serial.begin(9600); 
  // "attach"  servo 1 to the defined pin
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  // set the neutral, centre, position for the servo
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  // Alternte command for setting the neutral position
  //servo1.writeMicroseconds(1500);

  // wait until the servo has moved into position, (1000ms is more time than needed)
  delay(1000);
}

// -------------------------------------
// Loop Function
void loop()
{
   potReading1 = analogRead(potAnalogPin1);
   potReading2 = analogRead(potAnalogPin2);
   potReading3 = analogRead(potAnalogPin3);
   mappedReading1 = map(potReading1, 0, 1023, 0, 180);
   mappedReading2 = map(potReading2, 0, 1023, 90, 0);
   mappedReading3 = map(potReading3, 0, 1023, 0, 180);
 //  if  (mappedReading < 0){
  //    mappedReading2 = map(mappedReading, -150, 0, 180, 0);
 //   }
//   else{
  //  mappedReading2 = map(mappedReading, 0, 150, 0, 90);}
  // start from the nuetral position, (90 degrees from the fully clockwise position)
  servo1.write(mappedReading1);
  servo2.write(mappedReading2);
  servo3.write(mappedReading3);
}
