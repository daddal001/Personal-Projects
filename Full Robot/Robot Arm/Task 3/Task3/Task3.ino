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
float servo2min = -1.2;

unsigned long timeStamp;
unsigned long elapsedTime;
unsigned long controlRate = 15; // milliseconds


float positioncommand;
float cAngle1 = 0;
float cAngle2 = 0;
float inc = 0.02;
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
  timeStamp = millis();
}

// -------------------------------------
// Loop Function
void loop()
{
  potReading1 = analogRead(potAnalogPin1);
  potReading2 = analogRead(potAnalogPin2);
  potReading3 = analogRead(potAnalogPin3);
    
  servo1Angle = myMap(potReading1, -1.571, 1.571);
  servo2Angle = myMap(potReading2, servo2min, 1.571);
  servo3Angle = myMap(potReading3, -1.571, 1.571);

  elapsedTime = millis() - timeStamp;
  
  if ( elapsedTime > controlRate ) {
    cAngle1 = position(servo1Angle, cAngle1, inc);
    servo1PulseWidth = moveServo(servo1, refPulseWidth1, cAngle1);
    
    cAngle2 = position(servo2Angle, cAngle2, inc);
    servo2PulseWidth = moveServo(servo2, refPulseWidth2, cAngle2);

    if (cAngle2 < 0.1){
      servo3Angle = myMap(potReading3,  -1.571, (1.189*(sq(cAngle2)) + 3.3942*(cAngle2) + 1.10));
    }
    else{
      servo3Angle = myMap(potReading3, -1.571, 1.571);
      }
    servo3PulseWidth = moveServo(servo3, refPulseWidth3, servo3Angle);
    
    timeStamp = millis();
  }

  Serial.print("S1 (Base) = ");
  Serial.print(servo1Angle);
  Serial.print("rad (");
  Serial.print(servo1PulseWidth);
  Serial.print("us) | ");
  Serial.print("S2 (Elbow) = ");
  Serial.print(servo2Angle);
  Serial.print("rad (");
  Serial.print(servo2PulseWidth);
  Serial.print("us) | ");
  Serial.print("S3 (Wrist) = ");
  Serial.print(servo3Angle);
  Serial.print("rad (");
  Serial.print(servo3PulseWidth);
  Serial.println("us)");
}

float myMap(int potValue, float radAngleMin, float radAngleMax){
  float radAngle = potValue*((radAngleMax-radAngleMin)/1023)+radAngleMin;
  return radAngle;
}

int moveServo(Servo servo, int refPulseWidth, float servoRadAngle){
  const float Pi = 3.142;
  float minPulseWidth = float(refPulseWidth)-1000.0;
  int cmdSignal = (servoRadAngle+(Pi/2))*(2000.0/Pi)+minPulseWidth;
  servo.writeMicroseconds(cmdSignal);
  return cmdSignal;
}

float position (float TargetAngle, float CurrentAngle, float increment){
  
  if (TargetAngle > CurrentAngle) {
    positioncommand = min(TargetAngle, CurrentAngle + increment);
  }
  else if (TargetAngle < CurrentAngle) {
    positioncommand = max(TargetAngle, CurrentAngle - increment);
  }
  else {
    positioncommand = TargetAngle;
  }
  return positioncommand;
}