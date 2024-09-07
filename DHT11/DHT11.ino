#include <DHT.h>

// Define the DHT sensor pin and type
#define DHTPIN D4      // Pin where the DHT sensor is connected
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

void setup() {
  // Start the serial communication to output the data to the Serial Monitor
  Serial.begin(9600);
  Serial.println("DHT11 sensor reading...");
  
  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the results to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
}
