//This sketch is the Control LED Intensity Example from the Introduction to
// Arduino – PWM control of Actuators laboratory worksheet

// define a macro for the LED pin number 
#define LEDpin 11

// -------------------------------------
// Setup function
void setup() {
  // define the pin mode for the LED pin
  pinMode(LEDpin, OUTPUT);

}
// -------------------------------------
// Lop Function
void loop() {

  // Increment a variable, i, from 0 and 255
  for (int i = 0; i < 256; i++)
  {
    // modify the PWM signal, by passing the variable, i, to the analogWrite function
    analogWrite(LEDpin, i);
    //delay the program by 10ms
    delay(10);
  }

  // Decrement a variable, i, from 255 to 0
  for (int i = 255; i >= 0; i--)
  {
    // modify the PWM signal, by passing the variable, i, to the analogWrite function
    analogWrite(LEDpin, i);
    //delay the program by 10ms
    delay(10);
  }

}
