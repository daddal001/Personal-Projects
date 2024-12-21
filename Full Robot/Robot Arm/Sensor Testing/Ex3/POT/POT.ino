
 
int potAnalogPin = 1; // FSR is connected to analog 0
int LEDpin = 11;      // connect Red LED to pin 11 (PWM pin)
int potReading;      // the analog reading from the FSR resistor divider
int LEDbrightness;
int mappedReading;
 
void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  pinMode(LEDpin, OUTPUT);
}
 
void loop(void) {
  potReading = analogRead(potAnalogPin);
  mappedReading = map(potReading, 0, 1023, -150, 150);
  Serial.print("Mapped reading = ");
  Serial.println(mappedReading);
 
  delay(10);
}
