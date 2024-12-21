#include <Arduino.h>
#include <SD.h>

class Sensor {
  protected:
    int pin;
    bool sensorState = false;
    String sensorID;
    String roomID;
    bool SensorTriggered = false;
    int reading;

    bool getSensorState(void){
      return sensorState;
    }
    bool isSensorTriggered(void){
      return SensorTriggered;
    }
    void triggerSensor(void){
      SensorTriggered = true;
    }
    void resetSensor(void){
      SensorTriggered = false;
    }
    

  public:
     void turnON(void){
      sensorState = true;
    }
    void turnOFF(void){
      sensorState = false;
    }
    void setSensorPIN(int input_pin){
      pin = input_pin;
    }
    void setSensorID(String input_ID){
      sensorID = input_ID;
    }
    void setRoomID(String input_ID){
      roomID = input_ID;
    }
    sendreading(int input_reading){
      reading = input_reading;
      //Serial.println(reading);
    }
    int getSensorPIN(void){
      return pin;
    }
};

class motionSensor: public Sensor {
  public:
    bool monitorMotion(void)
    {
      if (Sensor::getSensorState() == true)
      {
        if (Sensor::isSensorTriggered() == true)
        {
          Sensor::triggerSensor();
          Sensor::resetSensor();
          Serial.println("Triggered motion");
          return true;
        }
        else
        {
          if (reading > 10)
          {
            Sensor::triggerSensor();
          }
        }
      }
      return false;
    }
};

class magnetSensor: public Sensor {
  public:
    bool monitorMagnet(void)
    {
      if (Sensor::getSensorState() == true)
      {
        if (Sensor::isSensorTriggered() == true)
        {
          Sensor::triggerSensor();
          Sensor::resetSensor();
          return true;
        }
        else
        {
          if (reading == 1)
          {
            Sensor::triggerSensor();
          }
        }
      }
      return false;
    }
};

int i;
motionSensor motion1;
magnetSensor magnet1;
int motion1Reading;
int magnet1Reading;

void setup() {
  pinMode(32, OUTPUT);
  Serial.begin(9600); 
  //motion1.turnON();
  magnet1.turnON();
  motion1.setSensorPIN(3);
  magnet1.setSensorPIN(11);
  pinMode(magnet1.getSensorPIN(), INPUT_PULLUP);
  pinMode(motion1.getSensorPIN(), INPUT);
  digitalWrite(motion1.getSensorPIN(), LOW);
  i = 0;
}

void loop() {
  motion1Reading = digitalRead(motion1.getSensorPIN());
  magnet1Reading = digitalRead(magnet1.getSensorPIN());
  motion1.sendreading(motion1Reading);
  motion1.monitorMotion();
  magnet1.sendreading(magnet1Reading);
  magnet1.monitorMagnet();
  if (magnet1Reading == 1 or motion1Reading == 1)
  {
    tone(32, 100, 20);
  }
  Serial.print(magnet1Reading);
  Serial.println(motion1Reading);
  //Serial.println(motion1.monitorMotion(), magnet1.monitorMagnet());
}