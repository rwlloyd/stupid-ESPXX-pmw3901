#include "Bitcraze_PMW3901.h"
#include <NewPing.h>

#define FLOW_CS 15 //CS pin for flow sensor
#define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Using digital pin 10 for chip select
Bitcraze_PMW3901 flow(15);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


int16_t deltaX, deltaY;
int sumX, sumY;
int distance = 0; //distance from sonar in cm

void setup() {
  Serial.begin(9600);

  if (!flow.begin()) {
    Serial.println("Initialization of the flow sensor failed");
    while (1) { }
  }
}

unsigned long previousMillis = 0;
const long interval = 50;

void loop() {
  unsigned long currentMillis = millis();
  
  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);
  sumX += deltaX;
  sumY += deltaY;
if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
  distance = int(sonar.ping_cm());
}
  Serial.print("Ping: ");
  Serial.print(distance); // Send ping, get distance in cm and print result (0 = outside set distance range)

  Serial.print(", X: ");
  Serial.print(deltaX);
  Serial.print(", Y: ");
  Serial.print(deltaY);

  Serial.print(", sumX: ");
  Serial.print(sumX);
  Serial.print(", sumY: ");
  Serial.print(sumY);
  Serial.print("\n");

  delay(100);
}
