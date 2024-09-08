#define BLYNK_PRINT Serial
#include <Wire.h>

//Replace with your template ID
#define BLYNK_TEMPLATE_ID "TMPL6v88lb0cy"
#define BLYNK_TEMPLATE_NAME "DHT11"
#define BLYNK_AUTH_TOKEN "blABbCsu4EPmlq8hsYPq0Fbl_MM7prid"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
// Define the DHT11 sensor type and the pin it's connected to
#define DHTTYPE DHT11
#define DHTPIN D4  // You can change this to the pin you're using

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SAAD";
char pass[] = "Pakistan";

BLYNK_WRITE(V2) {
  digitalWrite(D0, param.asInt());
}

BLYNK_WRITE(V3) {
  digitalWrite(D5, param.asInt());
}

void setup() {
  // Debug console
  Serial.begin(9600);
  dht.begin();
  pinMode(D0, OUTPUT);
  pinMode(D5, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  delay(3000);
}

void loop() {
  Blynk.run();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V0, temperature);

  delay(2000);  // Update every 2 seconds
}
