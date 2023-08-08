#include "Bitcraze_PMW3901.h"

// Using digital pin 15 for chip select
Bitcraze_PMW3901 flow(3);

int16_t deltaX, deltaY;
int sumX = 0;
int sumY = 0;

void setup() {
  Serial.begin(9600);

  if (!flow.begin()) {
    Serial.println("Initialization of the flow sensor failed");
    while (1) { }
  }
}



void loop() {
  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);
  sumX += deltaX;
  sumY += deltaY;
  Serial.print("dX: ");
  Serial.print(deltaX);
  Serial.print(", dY: ");
  Serial.print(deltaY);
  Serial.print(", sX: ");
  Serial.print(sumX);
  Serial.print(", sY: ");
  Serial.print(sumY);
  Serial.print("\n");

  delay(100);
}
