#include <Wire.h>

#define TRIGGER_PIN D3
#define ECHO_PIN D2

void setup() {
  Serial.begin(9600);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float getDistance() {
  // Send a 10us pulse to trigger the ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the pulse duration on the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm (speed of sound is 343 m/s)
  float distance_cm = (duration * 0.0343) / 2;
  return distance_cm;
}

void loop() {
  float distance = getDistance();
  
  // Clear the LCD and display the distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(1000); // Update every second
}
