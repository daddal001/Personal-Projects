#include <Arduino.h>
#include <SD.h>

const int ledPins[] = {16, 17}; // Pins for the LEDs
//const uint8_t redLEDPin = 13; // Pin for the red LED
//const uint8_t ledPin = 16; // Pin for the LED
const int motionSensorPin = 12; // Pin for the Motion Sensor
const int magneticSensorPin = 14; // Pin for the Magnatic Sensor
const int buzzerPin = 15;     // Pin for the Buzzer 
const int MAX_USERS = 10; // Maximum number of users
const int MAX_LOGS = 100; // Maximum number of system logs
const int USER_INFO_SIZE = 3; // Number of fields in user info (userID, password, userType)
const int NUM_ROOMS= 3; // Number of fields in user info (userID, password, userType)
const int MAX_ROOMS= 3; // Number of fields in user info (userID, password, userType)
  void readSystemLogs(); // Function declaration for readSystemLogs()
  void handleMotionSensor(); // Function declaration for handleMotionSensor()
  void handleMagneticSensor(); // Function declaration for handleMagneticSensor()
  void setBuzzerSoundLevel(int level); // Function declaration for setBuzzerSoundLevel()
const int password;

// Declare your variables here
bool motionSensorEnabled[MAX_ROOMS];
bool magneticSensorEnabled[MAX_ROOMS];
bool buzzerAlarmEnabled[MAX_ROOMS];
int buzzerSoundLevel[MAX_ROOMS];
bool ledEnabled[MAX_ROOMS];
int numUsers = 0;
String users[MAX_USERS][2]; // [userID, password]
int failedAttempts = 0;
int numLogs = 0;
String systemLogs[MAX_LOGS];
int redLEDPin = 13;/* your red LED pin number */;

class Sensor {
private:
    int motionpin;
    int magpin;

public:
    Sensor(int pin1, int pin2) : motionpin(pin1), magpin(pin2) {
        // Constructor
    }

    bool SensorState = false;
    int SensorIdMotion;
    int SensorIdMag;
    int RoomId;
    bool SensorTriggered = false;

    void MotionActivate(int on, int pin) {
        while (on == 1) {
            SensorState = true;
            if (digitalRead(pin) == HIGH) {
                SensorTriggered = true; // records whenever the sensor is triggered
            } else {
                digitalWrite(pin, LOW); // while nothing is being sensed the sensor is set to low
                SensorTriggered = false;
            }
        }
        SensorState = false;
        digitalWrite(pin, LOW); // keeps sensor in low state while sensor is not in use
    }

    void MagnetActivate(int on, int pin) {
        while (on == 1) {
            SensorState = true;
            if (digitalRead(pin) == HIGH) {
                SensorTriggered = true; // records whenever the sensor is triggered
            } else {
                digitalWrite(pin, LOW); // while nothing is being sensed the sensor is set to low
                SensorTriggered = false;
            }
        }
        SensorState = false;
        digitalWrite(pin, LOW); // keeps sensor in low state while sensor is not in use
    }
};

class Led{
private:
int Lpin;
public:
 Led(int Ledpin){
 Lpin = Ledpin;
 pinMode(Ledpin, OUTPUT);
 bool LEDState = false;
 }
 void Led::LedOn(){    // use array of objects
 digitalWrite(Lpin, HIGH);
bool LEDState = true;
}
void Led::LedOff(){
digitalWrite(Lpin, LOW);
bool LEDState = false;
}
};


class door
{
private:
int doorpin;
public:
door(int doorpinMode)
{
pinMode(doorpinMode,OUTPUT);
int doorpin;
bool doorstate = false;
}


void door::dooropen()
{
 digitalWrite(doorpin,LOW);
 bool doorstate = true;
}

void door::doorclosed(){
  digitalWrite(doorpin,HIGH);
 bool doorstate = false;
}
};

class Buzzer{
private:
int Bpin;
public:
Buzzer(int Buzpin){
pinMode(Buzpin,OUTPUT);
Bpin = Buzpin;
bool BuzzerState;
}
void Buzzer::BuzzerON(){
tone(Bpin, 1000);
bool BuzzerState = true;
}
void Buzzer::BuzzerOFF(){
 tone(Bpin, 1000);
bool BuzzerState = false;
}
};

class PanicButton{
private:
int panicpin;
public:
    PanicButton(int Panpin){
    pinMode(Panpin,INPUT);
    panicpin = Panpin;
    bool panicstate; 
  }
  void PanicButton::PanicButtonON(Buzzer &buz,Led &light){
    buz.BuzzerON();
    light.LedOn();
    bool panicstate = true;
  }
void PanicButton::PanicButtonOFF(Buzzer &buz,Led &light){
  light.LedOff();
  buz.BuzzerOFF();
  bool panicstate = false;
}
};


class RootUser 
{
public:
    static const int* ledPins;
    const int motionSensorPin = 12;  // Pin for the Motion Sensor
    const int magneticSensorPin = 14;  // Pin for the Magnetic Sensor
    const int buzzerPin = 15;  // Pin for the Buzzer
    static const int MAX_USERS = 10;
    static const int USER_INFO_SIZE = 3;
    static const int MAX_LOGS = 100;
    static const int NUM_ROOMS = 3;
    String userType = "Root";
    String userID;
    String password;
    static String users[MAX_USERS][USER_INFO_SIZE];  // Store user information as a 2D array
    int numUsers = 0;  // Number of users currently stored
    static String systemLogs[MAX_LOGS];  // Store system logs as a 1D array
    int numLogs = 0;  // Number of system logs currently stored
    int failedAttempts = 0;  // Counter for failed password attempts
    static bool motionSensorEnabled[NUM_ROOMS];  // true: on, false: off
    static bool magneticSensorEnabled[NUM_ROOMS];  // true: on, false: off
    static bool buzzerAlarmEnabled[NUM_ROOMS];  // true: on, false: off
    static bool ledEnabled[NUM_ROOMS];  // true: on, false: off
    int redLEDPin = 13;  // Pin for the red LED

    RootUser(const String& newUserID, const String& newPassword)
        : userID(newUserID), password(newPassword) {
        // Initialize device configuration arrays
        for (int i = 0; i < NUM_ROOMS; i++) {
            motionSensorEnabled[i] = false;
            magneticSensorEnabled[i] = false;
            buzzerAlarmEnabled[i] = false;
            ledEnabled[i] = false;
        }
        // Initialize pin modes
        pinMode(motionSensorPin, INPUT);
        pinMode(magneticSensorPin, INPUT);
        pinMode(buzzerPin, OUTPUT);
        for (int i = 0; i < NUM_ROOMS; i++) {
            pinMode(ledPins[i], OUTPUT);
        }
        pinMode(redLEDPin, OUTPUT);  // Initialize the red LED pin
    }

    void handleMagneticSensor() {
        // Handle magnetic sensor event
        Serial.println("Door/window opened or closed!");
    }

    void handleMotionSensor() {
        // Handle motion sensor event
        Serial.println("Motion detected!");
    }

    //bool manageUsers(); 
   // {
        // Modes
        // 1- Add user
        // 2- Remove user
        // 3- Change user password
   //     return true;
   // }
    
    bool manageAlarmModes() {
        // buzzers and LEDs
        // Implement alarm mode management logic here
        return true;
    }

    bool manageFacialRecognition() {
        // Implement facial recognition management logic here
        return true;
    }

    bool manageDevices() {
        // Select room(s)
        Serial.println("Select room(s) (1: Room 1, 2: Room 2, 3: Both):");
        int roomSelection = Serial.parseInt();

        // Configure devices for selected room(s)
        for (int i = 0; i < NUM_ROOMS; i++) {
            if (roomSelection == 1 && i == 0 || roomSelection == 2 && i == 1 || roomSelection == 3) {
                Serial.print("Room ");
                Serial.println(i + 1);

                // Motion sensor
                Serial.println("Enable motion sensor? (0: No, 1: Yes)");
                motionSensorEnabled[i] = Serial.parseInt() == 1;

                // Magnetic sensor
                Serial.println("Enable magnetic sensor? (0: No, 1: Yes)");
                magneticSensorEnabled[i] = Serial.parseInt() == 1;

                // Buzzer alarm
                Serial.println("Enable buzzer alarm? (0: No, 1: Yes)");
                buzzerAlarmEnabled[i] = Serial.parseInt() == 1;

                // LED
                Serial.println("Enable LED? (0: No, 1: Yes)");
                ledEnabled[i] = Serial.parseInt() == 1;
            }
        }

        return true;
    }

    void configureDevices(int room) {
        // Configure motion sensor
        if (motionSensorEnabled[room]) {
            Serial.println("Motion sensor enabled for Room " + String(room + 1));
            digitalWrite(motionSensorPin, HIGH);
        } else {
            Serial.println("Motion sensor disabled for Room " + String(room + 1));
            digitalWrite(motionSensorPin, LOW);
        }

        // Configure magnetic sensor
        if (magneticSensorEnabled[room]) {
            Serial.println("Magnetic sensor enabled for Room " + String(room + 1));
            digitalWrite(magneticSensorPin, HIGH);
        } else {
            Serial.println("Magnetic sensor disabled for Room " + String(room + 1));
            digitalWrite(magneticSensorPin, LOW);
        }

        // Configure buzzer alarm
        if (buzzerAlarmEnabled[room]) {
            Serial.println("Buzzer alarm enabled for Room " + String(room + 1));
        } else 
        {
            Serial.println("Buzzer alarm disabled for Room " + String(room + 1));
            noTone(buzzerPin);
        }

        // Configure LED
        if (ledEnabled[room]) {
            Serial.println("LED enabled for Room " + String(room + 1));
            digitalWrite(ledPins[room], HIGH);
        } else {
            Serial.println("LED disabled for Room " + String(room + 1));
            digitalWrite(ledPins[room], LOW);
        }
    }


   bool manageUsers() {
        // Modes
        // 1- Add user
        // 2- Remove user
        // 3- Change user password

        Serial.println("User Management:");
        Serial.println("1. Add User");
        Serial.println("2. Remove User");
        Serial.println("3. Change User Password");
        Serial.print("Enter your choice: ");

        int choice = Serial.parseInt();
              delay(100);
        switch (choice) {
            case 1: {
                String newUserID, newPassword, newUserType;

                Serial.print("Enter new user ID: ");
                newUserID = Serial.readStringUntil('\n');

                Serial.print("Enter new password: ");
                newPassword = Serial.readStringUntil('\n');

                Serial.print("Enter user type: ");
                newUserType = Serial.readStringUntil('\n');

                addUser(newUserID, newPassword, newUserType);
                break;
            }
            case 2: {
                String userID;

                Serial.print("Enter user ID to remove: ");
                userID = Serial.readStringUntil('\n');

                if (removeUser(userID)) {
                    Serial.println("User removed successfully.");
                } else {
                    Serial.println("User not found.");
                }
                break;
            }
            case 3: {
                String userID, newPassword;

                Serial.print("Enter user ID: ");
                userID = Serial.readStringUntil('\n');

                Serial.print("Enter new password: ");
                newPassword = Serial.readStringUntil('\n');

                if (changeUserPassword(userID, newPassword)) {
                    Serial.println("Password changed successfully.");
                } else {
                    Serial.println("User not found.");
                }
                break;
            }
            default:
                Serial.println("Invalid choice.");
                break;
        }

        return true;
    }

    void addUser(const String& newUserID, const String& newPassword, const String& newUserType) {
        if (numUsers < MAX_USERS) {
            users[numUsers][0] = newUserID;
            users[numUsers][1] = newPassword;
            users[numUsers][2] = newUserType;
            numUsers++;
            Serial.println("User added successfully.");
        } else {
            Serial.println("Maximum number of users reached.");
        }
    }

    bool removeUser(const String& userID) {
        for (uint8_t i = 0; i < numUsers; i++) {
            if (users[i][0] == userID) {
                // Shift the remaining users to overwrite the removed user
                for (uint8_t j = i + 1; j < numUsers; j++) {
                    users[j - 1][0] = users[j][0];
                    users[j - 1][1] = users[j][1];
                    users[j - 1][2] = users[j][2];
                }
                numUsers--;
                return true;
            }
        }
        return false; // User not found
    }

    bool changeUserPassword(const String& userID, const String& newPassword) {
        for (uint8_t i = 0; i < numUsers; i++) {
            if (users[i][0] == userID) {
                users[i][1] = newPassword;
                return true;
            }
        }
        return false; // User not found
    }
 

    bool authenticate(const String& enteredPassword) 
    {
        if (enteredPassword == password) {
            failedAttempts = 0;  // Reset failed attempts counter
            return true;
        } else {
            failedAttempts++;
            if (failedAttempts >= 3) 
            {
                digitalWrite(redLEDPin, HIGH);  // Turn on the red LED
                String rootPassword;
                while (rootPassword != password) {
                    rootPassword = getUserInput("Enter root password: ");
                }
                failedAttempts = 0;  // Reset failed attempts counter
            }
            return false;
    }
}
 
    void setBuzzerSoundLevel( int level) {
        // Set the buzzer sound level based on the provided level
        // (You can add your logic here)
        switch (level) {
            case 0:
                // Set soft sound level
                break;
            case 1:
                // Set medium sound level
                break;
            case 2:
                // Set loud sound level
                break;
            default:
                // Handle invalid sound level
                break;
        }
    }

private:
    String getUserInput(const String& prompt) {
        Serial.print(prompt);
        return Serial.readStringUntil('\n');
    }
};


const int ledPinsArray[] = {16, 17};
const int* RootUser::ledPins = ledPinsArray;
bool RootUser::motionSensorEnabled[NUM_ROOMS] = {false};
bool RootUser::magneticSensorEnabled[NUM_ROOMS] = {false};
bool RootUser::buzzerAlarmEnabled[NUM_ROOMS] = {false};
bool RootUser::ledEnabled[NUM_ROOMS] = {false};
String RootUser::users[MAX_USERS][USER_INFO_SIZE];
String RootUser::systemLogs[MAX_LOGS];




class User : public RootUser 
{

private:
    String userID;
    String password;
    String users[MAX_USERS][USER_INFO_SIZE]; // Store user information as a 2D array
    int numUsers = 0;
    int failedAttempts = 0; 
    
public:
int i;
String userstate;
bool passwordveri;
bool userveri;
bool faceveri;
bool systemActivate;
int attempt,b,a,j;

  bool passwordcheck(String password){
    for(int a = 0;a > i; a++){
      if(password = users[11][3]){
        passwordveri = true;
        return true;
      }
      else{
        passwordveri = false;
        attempt++;
        return false;
      }
    }
  }
bool User::usercheck(String username){
    for(int b = 0;b < i; a++){
      if(username == users[11][3]){
        userveri = true;
        return true;
      }
      else{
        userveri = false;
        attempt++;
        return false;
      }
    }
  }
bool User::Activate(char activate){
  if(activate = 'y'){
    systemActivate = true;
    return true; 
  }
  else if(activate = 'n'){
    systemActivate = false;
    return false;
  }
}
char User::Getuserid(String username){
  i = 0;
  while(username != users[i][3] ){
    i++;
    j++;
  }
userstate = users[j][2];
}
bool User::changeUserPassword(const String& userID, const String& newPassword) {
        for (int i = 0; i < numUsers; i++) {
            if (users[i][0] == userID) {
                users[i][1] = newPassword;
                return true;
            }
        }
        return false; // User not found
    }
};




class Session
 {
private:
    String userId;
    String sessionId;
    unsigned long sessionStartTime = 0;
    unsigned long lastSessionTime = 0;
    unsigned long sessionCounter = 0;

public:
    Session(const String& userId) : userId(userId) 
    {
        generateSessionId();
    }

    void generateSessionId() 
    {
        unsigned long currentTime = millis();
        if (currentTime - lastSessionTime >= 60000) 
        { // Generate a new session ID every minute
            sessionCounter++;
            lastSessionTime = currentTime;
            sessionStartTime = currentTime; // Record the start time of the new session
        }
        sessionId = String(sessionCounter, DEC);
        sessionId = String("0000000") + sessionId.substring(sessionId.length() - 7); // Pad with leading zeros
    }

    void sendOverSerial() {
        Serial.begin(9600); // Initialize serial communication
        Serial.print("User ID: ");
        Serial.println(userId);
        Serial.print("Session ID: ");
        Serial.println(sessionId);
        Serial.print("Date: ");
        Serial.print(getDate());
        Serial.print(" Time: ");
        Serial.println(getTime());
    }

    String getDate() 
    {
        unsigned long currentTime = millis() / 1000; // Convert to seconds
        unsigned long days = currentTime / 86400; // Seconds in a day
        unsigned long years = days / 365; // Approximate years
        days -= years * 365; // Remaining days
        unsigned long months = days / 30; // Approximate months
        days -= months * 30; // Remaining days
        String dateStr = String(years + 2000) + "/" + String(months + 1) + "/" + String(days + 1);
        return dateStr;
    }

    String getTime() 
    {
        unsigned long currentTime = millis() / 1000; // Convert to seconds
        unsigned long seconds = currentTime % 60;
        unsigned long minutes = (currentTime / 60) % 60;
        unsigned long hours = (currentTime / 3600) % 24;
        String timeStr = String(hours) + ":" + String(minutes) + ":" + String(seconds);
        return timeStr;
    }

    String getSessionDuration() 
    {
        unsigned long currentTime = millis();
        unsigned long sessionDuration = (currentTime - sessionStartTime) / 1000; // Convert to seconds
        unsigned long hours = sessionDuration / 3600;
        unsigned long minutes = (sessionDuration / 60) % 60;
        unsigned long seconds = sessionDuration % 60;
        String durationStr = String(hours) + ":" + String(minutes) + ":" + String(seconds);
        return durationStr;
    }

    void writeToFile() 
    {
        String fileName = "Log_" + sessionId + ".txt";
        File logFile = SD.open(fileName, FILE_WRITE);
        if (logFile)
         {
            logFile.print("User ID: ");
            logFile.println(userId);
            logFile.print("Session ID: ");
            logFile.println(sessionId);
            logFile.print("Date: ");
            logFile.println(getDate());
            logFile.print("Time: ");
            logFile.println(getTime());
            logFile.print("Session Duration: ");
            logFile.println(getSessionDuration());
            logFile.close();
            Serial.println("Data written to file: " + fileName);
        } 
        else 
        {
            Serial.println("Error opening " + fileName);
        }
    }
};
void printUsers(RootUser& rootUser) {
    Serial.println("User Information:");
    for (uint8_t i = 0; i < rootUser.numUsers; i++) {
        Serial.print("User ID: ");
        Serial.print(rootUser.users[i][0]);
        Serial.print(", Password: ");
        Serial.print(rootUser.users[i][1]);
        Serial.print(", User Type: ");
        Serial.println(rootUser.users[i][2]);
    }
}

void readSystemLogs(RootUser& rootUser) {
    if (SD.begin()) {
        File logFile = SD.open("systemlogs.txt", FILE_READ);
        if (logFile)
         {
            rootUser.numLogs = 0;  // Reset the log count
            while (logFile.available()) {
                rootUser.systemLogs[rootUser.numLogs] = logFile.readStringUntil('\n');
                rootUser.numLogs++;
            }
            logFile.close();
        } else {
            Serial.println("Error opening system log file.");
        }
    } else {
        Serial.println("SD card initialization failed.");
    }
}

void viewSystemLogs(RootUser& rootUser) {
    Serial.println("System Logs:");
    for (uint8_t i = 0; i < rootUser.numLogs; i++) {
        Serial.println(rootUser.systemLogs[i]);
    }
}

 RootUser rootUser("root", "rootpassword");

void setup() {

    Serial.begin(9600);

    // Add some initial users
    rootUser.addUser("admin", "password", "root");
    rootUser.addUser("user1", "pass123", "normal");
    rootUser.addUser("user2", "qwerty", "normal");

  //  rootUser.displayUsers(); // Display the initial list of users
}




void loop() 
{
    // Your main loop code here
 // Call manageUsers() to manage users
    rootUser.manageUsers();

    // Add a delay or other code here
    delay(100);


}














/*
bool RootUser::manageUsers() {
    // Modes
    // 1- Add user
    // 2- Remove user
    // 3- Change user password

    Serial.println("User Management:");
    Serial.println("1. Add User");
    Serial.println("2. Remove User");
    Serial.println("3. Change User Password");
    Serial.print("Enter your choice: ");

    int choice = Serial.parseInt();

    switch (choice) {
        case 1: {
            String newUserID, newPassword, newUserType;

            Serial.print("Enter new user ID: ");
            newUserID = Serial.readStringUntil('\n');

            Serial.print("Enter new password: ");
            newPassword = Serial.readStringUntil('\n');

            Serial.print("Enter user type: ");
            newUserType = Serial.readStringUntil('\n');

            addUser(newUserID, newPassword, newUserType);
            break;
        }
        case 2: {
            String userID;

            Serial.print("Enter user ID to remove: ");
            userID = Serial.readStringUntil('\n');

            if (removeUser(userID)) {
                Serial.println("User removed successfully.");
            } else {
                Serial.println("User not found.");
            }
            break;
        }
        case 3: {
            String userID, newPassword;

            Serial.print("Enter user ID: ");
            userID = Serial.readStringUntil('\n');

            Serial.print("Enter new password: ");
            newPassword = Serial.readStringUntil('\n');

            if (changeUserPassword(userID, newPassword)) {
                Serial.println("Password changed successfully.");
            } else {
                Serial.println("User not found.");
            }
            break;
        }
        default:
            Serial.println("Invalid choice.");
            break;
    }

    return true;
}

void addUser(const String& newUserID, const String& newPassword, const String& newUserType) {
    if (numUsers < MAX_USERS) {
        users[numUsers][0] = newUserID;
        users[numUsers][1] = newPassword;
        users[numUsers][2] = newUserType;
        numUsers++;
        Serial.println("User added successfully.");
    } else {
        Serial.println("Maximum number of users reached.");
    }
}

bool removeUser(const String& userID) {
    for (uint8_t i = 0; i < numUsers; i++) {
        if (users[i][0] == userID) {
            // Shift the remaining users to overwrite the removed user
            for (uint8_t j = i + 1; j < numUsers; j++) {
                users[j - 1][0] = users[j][0];
                users[j - 1][1] = users[j][1];
                users[j - 1][2] = users[j][2];
            }
            numUsers--;
            return true;
        }
    }
    return false; // User not found
}

bool changeUserPassword(const String& userID, const String& newPassword) {
    for (uint8_t i = 0; i < numUsers; i++) {
        if (users[i][0] == userID) {
            users[i][1] = newPassword;
            return true;
        }
    }
    return false; // User not found
}

void printUsers(RootUser& rootUser) {
    Serial.println("User Information:");
    for (uint8_t i = 0; i < rootUser.numUsers; i++) {
        Serial.print("User ID: ");
        Serial.print(rootUser.users[i][0]);
        Serial.print(", Password: ");
        Serial.print(rootUser.users[i][1]);
        Serial.print(", User Type: ");
        Serial.println(rootUser.users[i][2]);
    }
}
*/




/*
  // Authenticate root user
  if (rootUser.authenticate("rootpassword")) {
    Serial.println("Root user authenticated.");

    // Manage users
    if (rootUser.manageUsers()) {
      // User management operations performed
      printUsers(rootUser);  // Print updated user information
    }

    // Manage devices
    if (rootUser.manageDevices()) {
      // Device configuration operations performed
      for (int i = 0; i < rootUser.NUM_ROOMS; i++) {
        rootUser.configureDevices(i);
      }
    }

    // View system logs
    viewSystemLogs(rootUser);
  } else {
    Serial.println("Root user authentication failed.");
  }
*/
   
   /* Serial.begin(9600);

   

    if (rootUser.authenticate("rootpassword")) {
        Serial.println("Root user authenticated.");
        printUsers(rootUser);
        readSystemLogs(rootUser);
        viewSystemLogs(rootUser);

        if (rootUser.manageUsers()) {

           printUsers(rootUser);  // Print updated user information
        }

        if (rootUser.manageDevices()) {
            // Device configuration operations performed
            for (int i = 0; i < rootUser.NUM_ROOMS; i++) {
                rootUser.configureDevices(i);
            }
        }

        viewSystemLogs(rootUser);  // View system logs
    } else {
        Serial.println("Root user authentication failed.");
    }
     */

     // void addUser(const String& newUserID, const String& newPassword, const String& newUserType);
   //     if (numUsers < MAX_USERS) {
   //         users[numUsers][0] = newUserID;
   //         users[numUsers][1] = newPassword;
   //         users[numUsers][2] = newUserType;
   //         numUsers++;
   //     } else {
   //         Serial.println("Maximum number of users reached.");
   //     }
   // }

    //bool removeUser(const String& userID);
   // {
   //     for (uint8_t i = 0; i < numUsers; i++) {
   //         if (users[i][0] == userID) {
                // Shift the remaining users to overwrite the removed user
   //             for (uint8_t j = i + 1; j < numUsers; j++) {
   //                 users[j - 1][0] = users[j][0];
   //                 users[j - 1][1] = users[j][1];
   //                 users[j - 1][2] = users[j][2];
   //             }
   //             numUsers--;
   //             return true;
   //         }
   //     }
   //     return false;  // User not found
   // }

  // bool changeUserPassword(const String& userID, const String& newPassword);
  //      for (uint8_t i = 0; i < numUsers; i++) {
   //         if (users[i][0] == userID) {
   //             users[i][1] = newPassword;
  //              return true;
   //         }
   //     }
   //     return false;  // User not found
   // }