// THIS DOESNT SEEM TO WORK WITH PIMORONI BOARDS???


#include "Bitcraze_PMW3901.h"

// Using different chip select (CS) pins for each sensor
const int CS_PIN_1 = 15;
const int CS_PIN_2 = 3;

Bitcraze_PMW3901 flow1(CS_PIN_1);
Bitcraze_PMW3901 flow2(CS_PIN_2);

int16_t deltaX1, deltaY1;
int16_t deltaX2, deltaY2;
int sumX1 = 0;
int sumY1 = 0;
int sumX2 = 0;
int sumY2 = 0;

void setup() {
  Serial.begin(9600);

  if (!flow1.begin()) {
    Serial.println("Initialization of flow sensor 1 failed");
    while (1) { }
  }

  if (!flow2.begin()) {
    Serial.println("Initialization of flow sensor 2 failed");
    while (1) { }
  }
}

void loop() {
  // Get motion count from the first sensor since last call
  flow1.readMotionCount(&deltaX1, &deltaY1);
  sumX1 += deltaX1;
  sumY1 += deltaY1;

  // Get motion count from the second sensor since last call
  flow2.readMotionCount(&deltaX2, &deltaY2);
  sumX2 += deltaX2;
  sumY2 += deltaY2;

  Serial.print("Sensor 1 - dX: ");
  Serial.print(deltaX1);
  Serial.print(", dY: ");
  Serial.print(deltaY1);
  Serial.print(", sX: ");
  Serial.print(sumX1);
  Serial.print(", sY: ");
  Serial.print(sumY1);

  Serial.print(" | Sensor 2 - dX: ");
  Serial.print(deltaX2);
  Serial.print(", dY: ");
  Serial.print(deltaY2);
  Serial.print(", sX: ");
  Serial.print(sumX2);
  Serial.print(", sY: ");
  Serial.print(sumY2);

  Serial.println();
  
  delay(100);
}
