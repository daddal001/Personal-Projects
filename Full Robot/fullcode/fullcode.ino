// TB6612FNG Driver Board Sample board
//
// Author: Ben Taylor
// University of Sheffield
// Date: September 2021
//

int PWMValue = 0;

const int frontleft1Pin = 50; // Pin allocation for front left 1 Pin
const int frontleft2Pin = 52; // Pin allocation for front left 2 Pin
const int frontleftPWMPin = 4; // Pin allocation for the front left PWM pin
const int leftStandByPin = 48; // Pin allocation for the left standby pin
boolean frontleft1 = 0; // front left 1 value
boolean frontleft2 = 0; // front left 2 value
boolean leftstandBy = 0; // left standBy Value
boolean frontleftrotDirect = 0; // front left Rotation direction variable
unsigned char frontleftpwmValue = 0; // front left PWM value to be written to the output

const int frontright1Pin = 24; // Pin allocation for front right 1 Pin
const int frontright2Pin = 22; // Pin allocation for front right 2 Pin
const int frontrightPWMPin = 6; // Pin allocation for the front right PWM pin
const int rightStandByPin = 26; // Pin allocation for the right standby pin
boolean frontright1 = 0; // front right 1 value
boolean frontright2 = 0; // front right 2 value
boolean rightstandBy = 0; // right standBy Value
boolean frontrightrotDirect = 0; // front right Rotation direction variable
unsigned char frontrightpwmValue = 0; // front right PWM value to be written to the output

const int backleft1Pin = 46; // Pin allocation for back left 1 Pin
const int backleft2Pin = 44; // Pin allocation for back left 2 Pin
const int backleftPWMPin = 5; // Pin allocation for the back left PWM pin
boolean backleft1 = 0; // back left 1 value
boolean backleft2 = 0; // back left 2 value
boolean backleftrotDirect = 0; // back left Rotation direction variable
unsigned char backleftpwmValue = 0; // back left PWM value to be written to the output

const int backright1Pin = 28; // Pin allocation for back right 1 Pin
const int backright2Pin = 30; // Pin allocation for back right 2 Pin
const int backrightPWMPin = 7; // Pin allocation for the back right PWM pin
boolean backright1 = 0; // back right 1 value
boolean backright2 = 0; // back right 2 value
boolean backrightrotDirect = 0; // back right Rotation direction variable
unsigned char backrightpwmValue = 0; // back right PWM value to be written to the output

const int trig1Pin = 35; //Ultrasound 1 trig pin
const int echo1Pin = 31; //Ultrasound 1 echo pin

const int trig2Pin = 34; //Ultrasound 2 trig pin
const int echo2Pin = 36; //Ultrasound 2 echo pin

float duration, duration2, distance, distance2;

void setup() {
  // Ultrasound sensor pins
	pinMode(trig1Pin, OUTPUT);  
	pinMode(echo1Pin, INPUT);  

  pinMode(trig2Pin, OUTPUT);  
	pinMode(echo2Pin, INPUT);  

  // Assign the digital I/O pin directions
  pinMode(frontleft1Pin, OUTPUT);
  pinMode(frontleft2Pin, OUTPUT);
  pinMode(frontleftPWMPin, OUTPUT);
  pinMode(leftStandByPin, OUTPUT);

  pinMode(frontright1Pin, OUTPUT);
  pinMode(frontright2Pin, OUTPUT);
  pinMode(frontrightPWMPin, OUTPUT);
  pinMode(rightStandByPin, OUTPUT);

  pinMode(backleft1Pin, OUTPUT);
  pinMode(backleft2Pin, OUTPUT);
  pinMode(backleftPWMPin, OUTPUT);

  pinMode(backright1Pin, OUTPUT);
  pinMode(backright2Pin, OUTPUT);
  pinMode(backrightPWMPin, OUTPUT);
  //Initialize the serial port
  Serial.begin(9600);
  // Drive the standby pin high to enable the output
  leftstandBy = true;
  digitalWrite(leftStandByPin, leftstandBy);

  rightstandBy = true;
  digitalWrite(rightStandByPin, rightstandBy);
  // Set Initial values for AI1 and AI2 control function pins
  frontleft1 = true;
  frontleft2 = false;

  frontright1 = false;
  frontright2 = true;

  backleft1 = true;
  backleft2 = false;

  backright1 = false;
  backright2 = true;
  // set an initial value for the PWM value
  frontleftpwmValue= 0;//130;

  frontrightpwmValue= 0;//112;

  backleftpwmValue= 0;//148;

  backrightpwmValue= 0;//140;
}

void loop() {
  // Write the AI1 and AI2 values to the configuration pins
  digitalWrite(frontleft1Pin, frontleft1);
  digitalWrite(frontleft2Pin, frontleft2);

  digitalWrite(frontright1Pin, frontright1);
  digitalWrite(frontright2Pin, frontright2);

  digitalWrite(backleft1Pin, backleft1);
  digitalWrite(backleft2Pin, backleft2);

  digitalWrite(backright1Pin, backright1);
  digitalWrite(backright2Pin, backright2);
  // Write the pwnValue to the PWM pin
  analogWrite(frontleftPWMPin, frontleftpwmValue);

  analogWrite(frontrightPWMPin, frontrightpwmValue);

  analogWrite(backleftPWMPin, backleftpwmValue);

  analogWrite(backrightPWMPin, backrightpwmValue);
  // Display the board variable status to the Serial Monitor
  //Serial.print("PWM output value = ");
  //Serial.print(B1pwmValueA);
  //Serial.print(", Standby = ");
  //Serial.print(B1standBy);
  //Serial.print(", AI1 = ");
  //Serial.print(B1AI1);
  //Serial.print(", AI2 = ");
  //Serial.println(B1AI2);
  digitalWrite(trig1Pin, LOW);
  digitalWrite(trig2Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1Pin, HIGH);
  digitalWrite(trig2Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1Pin, LOW);
  digitalWrite(trig2Pin, LOW);

  duration = pulseIn(echo1Pin, HIGH);
  if ((duration*.0343)/2 < 800){
    distance = (duration*.0343)/2;}
  Serial.print("Distance: ");
  Serial.println(distance);

  //duration2 = pulseIn(echo2Pin, HIGH);
  //distance2 = (duration2*.0343)/2;
  //Serial.print(", Distance2: ");
  //Serial.println(distance2);

  // wait 250ms
  delay(100);
}