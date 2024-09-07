#include <ESP8266WiFi.h>

int D = 100;

void Connect_to_WiFi()
{
  WiFi.mode(WIFI_STA);  // Configure NodeMCU as station
  WiFi.begin("SAAD", "Pakistan"); // Wifi.begin(ssid, password)

  delay (1000);

  Serial.print("Connecting to Wifi!");

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(D4, 1);
    delay(D);
    digitalWrite(D4, 0);
    delay(D);
  }

  Serial.println("Wifi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void setup() {
  // put your setup code here, to run once:
  pinMode(D4, OUTPUT);

  delay (1000);

  Serial.begin(9600);
  
  Connect_to_WiFi();
}

void loop() {
  // put your main code here, to run repeatedly:

}
