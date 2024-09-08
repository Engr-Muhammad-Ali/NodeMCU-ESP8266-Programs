#define BLYNK_PRINT Serial
#include <Wire.h>

// Replace with your Blynk template details
#define BLYNK_TEMPLATE_ID "TMPL6Lcs5zTpp"
#define BLYNK_TEMPLATE_NAME "Ultrasonic Sensor"
#define BLYNK_AUTH_TOKEN "1M1A53h0woTWV8UZomdN2_Sj_PTtJpEo"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define the Ultrasonic Sensor pins
#define TRIGGER_PIN D3
#define ECHO_PIN D2

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SAAD";
char pass[] = "Pakistan";

void setup() {
  // Debug console
  Serial.begin(9600);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  delay(3000);
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
  Blynk.run();

  float distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send distance to Blynk app
  Blynk.virtualWrite(V0, distance); // V1 is the virtual pin for distance

  delay(1000); // Update every second
}
