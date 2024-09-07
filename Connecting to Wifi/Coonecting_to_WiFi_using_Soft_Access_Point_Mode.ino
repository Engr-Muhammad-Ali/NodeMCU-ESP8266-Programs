#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>

WiFiManager wifi;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wifi.autoConnect("NodeMCU ESP8266");

  Serial.println("Connected to NodeMCU ESP8266");
}

void loop() {
  // put your main code here, to run repeatedly:

}
