//Include libaries
#include <Arduino.h>
#include <SD.h>

//Define classes
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
    void triggerSensor(void){
      SensorTriggered = true;
    }
    void resetSensor(void){
      SensorTriggered = false;
    }
    

  public:
    bool isSensorTriggered(void){
      return SensorTriggered;
    }
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
    void sendreading(int input_reading){
      reading = input_reading;
    }
    int getSensorPIN(void){
      return pin;
    }
    String getRoomID(void){
      return roomID;
    }
    String getSensorID(void){
      return sensorID;
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

class buzzer{
  private:
    int pin;
  public:
    void setBuzzerPIN(int input_pin){
      pin = input_pin;
      pinMode(pin, OUTPUT);
    }
    void turnON(void){
      tone(pin, 1500, 20); 
    }
    void turnOFF(void){
      digitalWrite(pin , LOW);
    }
    int getBuzzerPIN(void){
      return pin;
    }

};

class LED{
  private:
    int pin;
  public:
    void setLEDPIN(int input_pin){
      pin = input_pin;
      pinMode(pin, OUTPUT);
    }
    void turnON(void){
      digitalWrite(pin , HIGH);
    }
    void turnOFF(void){
      digitalWrite(pin , LOW);
    }
    int getLEDPIN(void){
      return pin;
    }
};

class panicButton{
  private:
    int pin;
    int state;
    int oldState = 0;
  public:
    void setpanicPIN(int input_pin){
      pin = input_pin;
      pinMode(pin, INPUT);
    }

    bool panic(void){
      bool set;
      state = digitalRead(pin);
      if (state == HIGH)
      {
        return true;
      }
      return false;
    }
};

class door{
  private:
    int pin;
  public:
    void setdoorPIN(int input_pin){
      pin = input_pin;
      pinMode(pin, OUTPUT);
    }
    void doorOpen(void){
      analogWrite(pin,255);
    }
    void doorClose(void){
      analogWrite(pin,0);
    }
};

class user{
  private:
    String userType = "General";
    int userID;
    String password;
  public:
    bool verifyPassword(String input_password){
      if (input_password == password){
        return true;
      }
      else{
        return false;
      }
    }
    String getPassword(void){
      return password;
    }
    void changePassword(String new_password){
      password = new_password;
    }
    void setUserID(int ID){
      userID = ID;
    }
};


//Assign calsses
motionSensor motion1;
magnetSensor magnet1;
motionSensor motion2;
magnetSensor magnet2;
buzzer buzzer1;
LED green;
LED yellow;
LED red;
panicButton panicButton;
door door;

int motion1Reading;
int magnet1Reading;
int panicReading;

String mode1[4];
String mode2[4];

user *users = new user[100];
int numOfUsers = 0;

class root: public user{
  private:
    String userType = "root";
  public:
    void addUser(String password){
      users[numOfUsers].setUserID(numOfUsers);
      users[numOfUsers].verifyPassword(password);
      numOfUsers ++; 
    }
    void rmUser(String password){
      for(int i = 0; i < 5; i++){
        if(users[i].verifyPassword(password)){
          users[i].changePassword("aaaa");
        }
      }
    }
    void manageDevices(String type, bool state){
      if (type == "LED"){
        if(state == true){
          green.turnON();
          yellow.turnON();
          red.turnON();
        }
        else{
          green.turnOFF();
          red.turnOFF();
          yellow.turnOFF();
        }
      }
      else{
        if(state == true){
          buzzer1.turnON();
        }
        else{
          buzzer1.turnOFF();
        }
      }
    }
    void manageModes(String mode, String sensor, int index){
      if(mode == "mode1"){
        mode1[index] = sensor;
      }
      if(mode == "mode2"){
        mode2[index] = sensor;
      }
    }
};

root root1;
String inputch;
String inputstring;
int idchnum = 0;
String rootpass;
String genpass;
String data;
int out;
int x;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);

  root1.setUserID(0);
  root1.changePassword("1234");

  buzzer1.setBuzzerPIN(32);
  motion1.setSensorPIN(3);
  magnet1.setSensorPIN(11);
  green.setLEDPIN(51);
  yellow.setLEDPIN(50);
  red.setLEDPIN(44);
  panicButton.setpanicPIN(8);
  door.setdoorPIN(6);

  pinMode(magnet1.getSensorPIN(), INPUT_PULLUP);
  pinMode(motion1.getSensorPIN(), INPUT);

  digitalWrite(motion1.getSensorPIN(), LOW);
  door.doorOpen();

  motion1.setSensorID("01");
  motion2.setSensorID("02");
  magnet1.setSensorID("03");
  magnet2.setSensorID("04");

  magnet1.setRoomID("11");
  magnet2.setRoomID("22");
  motion1.setRoomID("11");
  motion2.setRoomID("22");
  String ALL[] = {motion1.getRoomID() + motion1.getSensorID(), motion2.getRoomID() + motion2.getSensorID(), magnet1.getRoomID() + magnet1.getSensorID(), magnet2.getRoomID() + magnet2.getSensorID()};
}

void loop() {
  while (!Serial.available());
  rootpass = root1.getPassword();
  data = getData();
  Serial.println(data);
  if (data == "3"){
    activate("all");
  }
  if (data == "2"){
    activate("mode 2");
  }
  if (data == "1"){
    activate("mode 1");
  }
  if (data == "4"){
    deactivate();
  }
  root1.manageModes("mode1", "magnet1", 0);
  root1.manageModes("mode2", "motion1", 0);
  activate("mode1");
  motion1Reading = digitalRead(motion1.getSensorPIN());
  magnet1Reading = digitalRead(magnet1.getSensorPIN());
  panicReading = panicButton.panic();

  motion1.sendreading(motion1Reading);
  motion1.monitorMotion();
  magnet1.sendreading(magnet1Reading);
  magnet1.monitorMagnet();
  
  if (magnet1.isSensorTriggered() == 1 or panicReading == true or motion1.isSensorTriggered())//or motion1Reading == 1)
  {
    delay(3000);
    triggerAlarm();
    //delay(10000);
  }
  //Serial.println(motion1.monitorMotion(), magnet1.monitorMagnet());
  if (magnet1.isSensorTriggered() == 1 || panicReading == true || motion1.isSensorTriggered()) {
        unsigned long currentTime = millis();
        addLogEntry(1, currentTime, 5000, 123); // Add log entry for user 1
        triggerAlarm();
         printLog();
    }
}

void triggerAlarm(void){
  unsigned long currentTime = millis();
  while((millis() - currentTime) < 7000){
    buzzer1.turnON();
    red.turnON();
    yellow.turnON();
    green.turnON();
    delay(10);
    red.turnOFF();
    green.turnOFF();
    yellow.turnOFF();
    delay(10);
  }
  buzzer1.turnOFF();
  red.turnOFF();
  green.turnOFF();
  yellow.turnOFF();
}

void activate(String mode){
  if(mode == "all"){
      motion1.turnON();
      magnet1.turnON();
  }
  else if(mode == "mode1"){
    for (int i = 0; i < 4; i++){
      if(mode1[i] == "magnet1"){
        magnet1.turnON();
      }
      if(mode1[i] == "motion1"){
        motion1.turnON();
      }
    }
  }
  else if(mode == "mode2"){
    for (int i = 0; i < 4; i++){
      if(mode2[i] == "magnet1"){
        magnet2.turnON();
      }
      if(mode2[i] == "motion1"){
        motion2.turnON();
      }
    }
  }
  delay(3000);
  door.doorClose();
}

void deactivate(){
  motion1.turnOFF();
  magnet1.turnOFF();
  door.doorOpen();
}

String getData(){
  inputch = "";
  while (!Serial.available());
  inputstring = Serial.readStringUntil('\r');
  //inputch = inputch + inputstring;
  inputch = inputstring;
  return inputch;
}

// Define the maximum number of log entries
#define MAX_LOG_ENTRIES 100

// Struct to store log entry details
struct LogEntry {
    int userID;
    unsigned long timestamp;
    unsigned long duration;
    int sessionID;
};

// Array to store log entries
LogEntry logEntries[MAX_LOG_ENTRIES];
int logEntryCount = 0;

void addLogEntry(int userID, unsigned long timestamp, unsigned long duration, int sessionID) {
    if (logEntryCount < MAX_LOG_ENTRIES) {
        logEntries[logEntryCount].userID = userID;
        logEntries[logEntryCount].timestamp = timestamp;
        logEntries[logEntryCount].duration = duration;
        logEntries[logEntryCount].sessionID = sessionID;
        logEntryCount++;
    } else {
        Serial.println("Maximum log entries reached. Unable to add more entries.");
    }
}


void printLog() {
    for (int i = 0; i < logEntryCount; i++) {
        Serial.print("User ID: ");
        Serial.print(logEntries[i].userID);
        Serial.print(", Timestamp: ");
        Serial.print(logEntries[i].timestamp);
        Serial.print(", Duration: ");
        Serial.print(logEntries[i].duration);
        Serial.print(", Session ID: ");
        Serial.println(logEntries[i].sessionID);
    }
}

