// https://api.thingspeak.com/update?api_key=QNC1QLGPJ8K29Z71&field1=0  // Writing data on ThingSpeak Channel Feed

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

int httpcode;
String URL;
String API = "QNC1QLGPJ8K29Z71&field1";
String FieldNo = "1";

WiFiClient client;
HTTPClient http;
WiFiManager wifi;

void Send_Data_to_ThingSpeak (int Data);  // Method for Sending data to ThingSpeak

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wifi.autoConnect("NodeMCU ESP8266");

  Serial.println("Connected to NodeMCU ESP8266");
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void Send_Data_to_ThingSpeak (int Data) // Definition of Method
{
  URL = "http://api.thingspeak.com/update?api_key=";
  URL = URL + API;  // Concatinating Strings
  URL = URL + "&field";
  URL = URL + FieldNo;
  URL = URL + "=";
  URL = URL + Data;

  http.begin(client, URL);  // https format not supported by NodeMCU ESP8266

  Serial.println("Waiting for ThingSpeak Server");

  httpcode = http.GET();

  if (httpcode > 0)
  {
    Serial.println("Data Sent Successfully!");
  }
  else
  {
    Serial.println("Error Sending Data!");
  }

  http.end();
  delay (16000);  // Time Taken by Free ThingSpeak Account to Load Data
}
