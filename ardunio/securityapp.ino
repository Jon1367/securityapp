#include "pitches.h";
#include <SPI.h>
#include <Ethernet.h>

// general
int interval = 100;
unsigned long previous = 0;
int calibrateTime = 15000;
int alertStatus = 0;
int isCalibrated = 0;
unsigned long timerStart = 0;
unsigned long timerLen = 0;

// pins
int speaker = 5;
int button = 6;
int redLED = 7;
int greenLED = 8;
int pir = 2;

// audio
unsigned long songPrev = 0;
int songInterval = 250;
int melody = NOTE_DS8;
int noteDurations = 2;

// flashes
unsigned long flashPrev = 0;
int flashInterval = 250;

// Ethernet
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x98, 0xAD };
EthernetClient client;

// this plays our alert audio
void playSong() {
  if (millis() >= songPrev + songInterval) {
    songPrev = millis();
    noTone(speaker);
  } else {
    int noteDuration = 1000 / noteDurations;
    tone(speaker, melody, noteDuration);
  }
}

// flashes the red led when the alarm is triggered
void flashRed() {
  digitalWrite(greenLED, LOW);
  if (millis() >= flashPrev + flashInterval) {
    flashPrev = millis();
    digitalWrite(redLED, LOW);
  } else {
    digitalWrite(redLED, HIGH);
  }
}

// initialize the alert
void startAlert() {
  playSong();
  flashRed();
}

// stop the alert
void stopAlert() {
  songPrev = 0;
  flashPrev = 0;
  if (isCalibrated == 0) {
    logData();
    sensorCalibration();
  }
}

// calibrate the sensor
void sensorCalibration() {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, HIGH);
  digitalWrite(pir, LOW);
  Serial.println("Calibrating Sensor.");
  delay(calibrateTime);
  Serial.println("Calibration Complete.");
  isCalibrated = 1;
  timerStart = 0;
  timerLen = 0;
  digitalWrite(redLED, LOW);
}

// sends an SMS to the user
void notifyUser() {
  char textServer[] = "securityapp-jon13671.c9.io";
  Serial.println("SMS Sent.");

  client.stop();
  
  if (client.connect(textServer, 80)) {
    client.print("GET ");
    client.print("/getData/1");
    client.print(timerLen);
    client.println(" HTTP/1.1");
    client.println("Host: securityapp-jon13671.c9.io");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
  
}

// Calibrates the internet
void internetCalibration() {
  Ethernet.begin(mac);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

// pushes the data to the database
void logData() {
  char dataServer[] = "data.sparkfun.com";
  timerLen = millis() - timerStart;
  Serial.println("Logging Data.");

  client.stop();
  
  if (client.connect(dataServer, 80)) {
    client.print("GET ");
    client.print("/input/q5y4E069JJTqQmlO8L0R?private_key=BVx0kW2yBBIy7mVDezWd&len=");
    client.print(timerLen);
    client.println(" HTTP/1.1");
    client.println("Host: data.sparkfun.com");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
}

void setup() {
  Serial.begin(57600);
  
  pinMode(speaker, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(pir, INPUT);

  sensorCalibration();
  internetCalibration();
}

void loop() {

  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  
  if (millis() >= previous + interval) {
    int btnState = digitalRead(button);
    previous = millis();

    if (btnState == 0 && alertStatus == 1) {
      alertStatus = 0;
    }
    
    if (alertStatus == 1) {
        startAlert();
    } else {
        stopAlert();
    }

    if(digitalRead(pir) == HIGH){
      if (alertStatus == 0) {
        timerStart = millis();
        isCalibrated = 0;
        alertStatus = 1;
        notifyUser();
      }      
    }
        
  } // end of interval checker
  
} // end of loop
