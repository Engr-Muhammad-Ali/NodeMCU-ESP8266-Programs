/*Temperature and Humidity monitoring system with Thingspeak
 * https://srituhobby.com
 */
 
#include <ESP8266WiFi.h>
#include "DHT.h"

String apiKey = "QNC1QLGPJ8K29Z71";
const char *ssid =  "SAAD";
const char *pass =  "Pakistan";
const char* server = "api.thingspeak.com";

DHT dht(D4, DHT11);

WiFiClient client;

void setup() {
  Serial.begin(9600);
  delay(10);
  dht.begin();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" °C \t");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %");
  }
  client.stop();
  delay(1000);
}
