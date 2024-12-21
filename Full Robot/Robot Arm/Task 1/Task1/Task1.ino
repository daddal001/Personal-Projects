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
float servo1Angle;
float servo2Angle;
float servo3Angle;
int servo1PulseWidth;
int servo2PulseWidth;
int servo3PulseWidth;
float refPulseWidth1 = 1504;
float refPulseWidth2 = 1497;
float refPulseWidth3 = 1557;
float servo2min = -0.88;
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
  servo2.write(81);
  servo3.write(87);
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
    
  servo1Angle = map(potReading1, 0, 1023, 0, 180);
  servo2Angle = map(potReading2, 0, 1023, -9, 171);
  servo3Angle = map(potReading3, 0, 1023, -3, 177);

  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);

  Serial.print("S1 (Base) = ");
  Serial.print(servo1Angle);
  Serial.print("rad | ");
  Serial.print("S2 (Elbow) = ");
  Serial.print(servo2Angle);
  Serial.print("rad | ");
  Serial.print("S3 (Wrist) = ");
  Serial.print(servo3Angle);
  Serial.println("rad | ");
}
