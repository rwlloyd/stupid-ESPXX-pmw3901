#include <NewPing.h> // For HC-SR04 Sensor
#include "network.h"
#include "Bitcraze_PMW3901.h"
#include "variables.h"

// Using digital pin 15 for chip select, instatiate flow sensor library
Bitcraze_PMW3901 flow(15);

// Yes, I know there are references to these variables in the variables.h file
// But this is where we initialise them, the .h is just so that the websocket code can see it.... i think.
int16_t deltaX = 0;
int16_t deltaY = 0;
int sumX = 0;
int sumY = 0;
int distance = 0;

// Set up the ultrasonic sensor
#define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define COUNT_RESET_PIN 2 // pin for a button

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600);

  pinMode(COUNT_RESET_PIN, INPUT_PULLUP);

  if (!flow.begin()) {
    Serial.println("Initialization of the flow sensor failed");
    while (1) { }
  }
  startNetwork(); // There's a fair bit of code for the networking stuff. see network.cpp
}

const unsigned long pingInterval = 50; //Basic update interval for sonar data
unsigned long pingPreviousMillis = 0;

void loop() {
  //Ping the flow surface?
  unsigned long pingCurrentMillis = millis();
  if (pingCurrentMillis - pingPreviousMillis >= pingInterval) {
    pingPreviousMillis = pingCurrentMillis;
    distance = int(sonar.ping_cm());
  }
  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);
  sumX += deltaX;
  sumY += deltaY;

  if (!digitalRead(COUNT_RESET_PIN)){
    sumX = 0;
    sumY = 0;
  }

  doNetworking();

  //  Serial.print("dX: ");
  //  Serial.print(deltaX);
  //  Serial.print(", dY: ");
  //  Serial.print(deltaY);
  //  Serial.print(", sX: ");
  //  Serial.print(sumX);
  //  Serial.print(", sY: ");
  //  Serial.print(sumY);
  //  Serial.print("\n");

}
