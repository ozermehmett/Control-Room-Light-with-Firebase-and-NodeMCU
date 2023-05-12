/*
1-Go to Firebase.

2-Create a new project.

3-Go to the "Rules" tab of the new project and make the following changes.

`{
  "rules": {
    ".read": true,
    ".write": true
  }
}
`

4-Switch to the "Data" tab and create a path named "/homeAsistant/durum".

5-Add a data point under the created path and assign a value representing the initial state (e.g., "0" or "1")
*/






#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <Servo.h>

Servo servo;

#define FIREBASE_HOST ""   // Paste your Firebase link here
#define FIREBASE_AUTH ""   // Paste your Firebase authentication ID here
#define WIFI_SSID ""  // Enter your WiFi SSID here
#define WIFI_PASSWORD ""   // Enter your WiFi password here

FirebaseData veritabanim;
String beforeOn = "1";
String beforeOff = "1";

void setup()
{
  servo.attach(5); // Connect the servo motor to pin D1
  servo.write(90); // Set the servo motor to the initial position
  delay(500);
  Serial.begin(115200); // Start serial communication
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to the network");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("Connected to the IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Connect to Firebase
  Firebase.reconnectWiFi(true); // Enable reconnection to WiFi if it gets disconnected
}

void loop()
{
  // Retrieve data from the Firebase database at "/homeAsistant/durum" path
  if (Firebase.getString(veritabanim, "/homeAsistant/durum"))
  {
    if (beforeOff == "1")
    {
      // If the light was off and the received data is "1", turn on the light
      if (veritabanim.stringData() == "1")
      {
        servo.write(120); // Adjust the servo motor to the appropriate angle to turn on the light
        delay(1000);
        servo.write(90); // Set the servo motor back to the initial position
        beforeOff = "0";
        beforeOn = "1";
      }
    }
    else if (beforeOn == "1")
    {
      // If the light was on and the received data is "0", turn off the light
      if (veritabanim.stringData() == "0")
      {
        servo.write(60); // Adjust the servo motor to the appropriate angle to turn off the light
        delay(1000);
        servo.write(90); // Set the servo motor back to the initial position
        beforeOff = "1";
        beforeOn = "0";
      }
    }
    else if (veritabanim.stringData() == "3")
    {
      // If the received data is "3", reset the light state
      beforeOff = "1";
      beforeOn = "1";
    }
  }
  delay(200);
}
