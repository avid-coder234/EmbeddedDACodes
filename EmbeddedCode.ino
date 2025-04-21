#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#define WIFI_SSID "realme X7 5G"
#define WIFI_PASSWORD "arpan123##"

#define API_KEY "apikey"
#define DATABASE_URL "databaseurl"

FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig firebaseConfig;
Servo servo1;
Servo servo2;
int servo1Pin = 13;
int servo2Pin = 12;
String databasePath1 = "/servoControl";
String databasePath2 = "/servoControl1";
int irSensor1Pin = 32;
int irSensor2Pin = 33;
int irSensor3Pin = 26;
int irSensor4Pin = 25;
String irSensor1Path = "/irsensor1";
String irSensor2Path = "/irsensor2";
String irSensor3Path = "/irsensor3";
String irSensor4Path = "/irsensor4";
void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  firebaseConfig.api_key = API_KEY;
  firebaseConfig.database_url = DATABASE_URL;
  if (Firebase.signUp(&firebaseConfig, &firebaseAuth, "", "")) {
    Serial.println("Signed up successfully");
  } else {
    Serial.println("Failed to sign up");
    Serial.println(firebaseData.errorReason());
    while (true);
  }
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
  
  if (Firebase.ready()) {
    Serial.println("Firebase connected successfully.");
  } else {
    Serial.println("Firebase initialization failed.");
    Serial.println("Firebase error: " + firebaseData.errorReason());
    while (true);
  }
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(0);
  servo2.write(0);
  
  pinMode(irSensor1Pin, INPUT);
  pinMode(irSensor2Pin, INPUT);
  pinMode(irSensor3Pin, INPUT);
  pinMode(irSensor4Pin, INPUT);
}
void loop() {
  if (Firebase.getInt(firebaseData, databasePath1)) {
    int servoControl1 = firebaseData.intData();
    Serial.println("Servo 1 control value: " + String(servoControl1));
    if (servoControl1 == 0) {
      servo1.write(0);
    } else if (servoControl1 == 1) {
      servo1.write(90);
    }
  } else {
    Serial.println("Failed to get value for Servo 1 from Firebase: " + firebaseData.errorReason());
  }
  if(digitalRead(irSensor3Pin)==0)
  {
    Firebase.setInt(firebaseData,databasePath2,1);
    servo2.write(90);
    delay(4000);
    servo2.write(0);
    Firebase.setInt(firebaseData,databasePath2,0);
  }
  if (Firebase.getInt(firebaseData, databasePath2)) {
    int servoControl2 = firebaseData.intData();
    Serial.println("Servo 2 control value: " + String(servoControl2));
    if (servoControl2 == 0) {
      servo2.write(0);
    } else if (servoControl2 == 1) {
      servo2.write(90);
    }
  } else {
    Serial.println("Failed to get value for Servo 2 from Firebase: " + firebaseData.errorReason());
  }
  int irSensor1Status = digitalRead(irSensor1Pin);
  int irSensor2Status = digitalRead(irSensor2Pin);
  int irSensor3Status = digitalRead(irSensor3Pin);
  int irSensor4Status = digitalRead(irSensor4Pin);
  if (Firebase.setInt(firebaseData, irSensor1Path, irSensor1Status)) {
    Serial.println("Updated IR sensor 1 status to: " + String(irSensor1Status));
  } else {
    Serial.println("Failed to update IR sensor 1: " + firebaseData.errorReason());
  }
  if (Firebase.setInt(firebaseData, irSensor2Path, irSensor2Status)) {
    Serial.println("Updated IR sensor 2 status to: " + String(irSensor2Status));
  } else {
    Serial.println("Failed to update IR sensor 2: " + firebaseData.errorReason());
  }
  if (Firebase.setInt(firebaseData, irSensor2Path, irSensor4Status)) {
    Serial.println("Updated IR sensor 2 status to: " + String(irSensor4Status));
  } else {
    Serial.println("Failed to update IR sensor 2: " + firebaseData.errorReason());
  }
delay(2000);
}
