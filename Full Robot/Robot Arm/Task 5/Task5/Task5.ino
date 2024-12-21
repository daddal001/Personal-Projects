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
// Declare a servo motor class variable servo 1
Servo servo1;
Servo servo2;
Servo servo3;

float angle1[] = {-1.00, 0, 0.79, 0, -0.32, 0, -0.94, 0, -0.32, 0};
float angle2[] = {-0.15, 0, -0.03, 0, -0.02, 0, -0.02, 0, -0.02, 0};
float angle3[] = {1.01, 0, 1.22, 0, 1.57, 0, 1.57, 0, 1.57, 0};

float positioncommand;
float cAngle1 = 0;
float cAngle2 = 0;
float cAngle3 = 0;
float inc = 0.005;

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
  delay(2000);
  timeStamp = millis();
}

// -------------------------------------
// Loop Function

void loop()
{
  
  for (int i; i < 10;){
    servo1Angle = angle1[i];
    servo2Angle = angle2[i];
    servo3Angle = angle3[i];

    elapsedTime = millis() - timeStamp;
    
    if ( elapsedTime > controlRate ) {
      cAngle1 = position(servo1Angle, cAngle1, inc);
      servo1PulseWidth = moveServo(servo1, refPulseWidth1, cAngle1);
      
      cAngle2 = position(servo2Angle, cAngle2, inc);
      servo2PulseWidth = moveServo(servo2, refPulseWidth2, cAngle2);

      cAngle3 = position(servo3Angle, cAngle3, inc);
      servo3PulseWidth = moveServo(servo3, refPulseWidth3, cAngle3);
      
      timeStamp = millis();
    }
    if (cAngle1 == servo1Angle && cAngle2 == servo2Angle && cAngle3 == servo3Angle){
      i ++;
      delay(2000);
    }
  }
  delay(2000);
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
