#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*Put WiFi SSID & Password*/
const char* ssid = "SAAD";   // Enter SSID here
const char* password = "Pakistan"; // Enter Password here

ESP8266WebServer server(80);  // Standard port for HTTP Server

bool greenLEDStatus = LOW;
bool blueLEDStatus = LOW;

void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(D0, OUTPUT);
  pinMode(D5, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check NodeMCU is connected to Wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());

  // Whenever the following URLs will be called, the specified methods will be invoked
  server.on("/", handle_OnConnect);
  server.on("/ledongreen", handle_ledongreen);
  server.on("/ledoffgreen", handle_ledoffgreen);
  server.on("/ledonblue", handle_ledonblue);
  server.on("/ledoffblue", handle_ledoffblue);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();

  digitalWrite(D0, greenLEDStatus);
  digitalWrite(D5, blueLEDStatus);
}

void handle_OnConnect() {
  greenLEDStatus = LOW;
  blueLEDStatus = LOW;
  Serial.println("Green LED: OFF, Blue LED: OFF");
  server.send(200, "text/html", updateWebpage()); 
}

void handle_ledongreen() {
  greenLEDStatus = HIGH;
  Serial.println("Green LED: ON");
  server.send(200, "text/html", updateWebpage()); 
}

void handle_ledoffgreen() {
  greenLEDStatus = LOW;
  Serial.println("Green LED: OFF");
  server.send(200, "text/html", updateWebpage()); 
}

void handle_ledonblue() {
  blueLEDStatus = HIGH;
  Serial.println("Blue LED: ON");
  server.send(200, "text/html", updateWebpage()); 
}

void handle_ledoffblue() {
  blueLEDStatus = LOW;
  Serial.println("Blue LED: OFF");
  server.send(200, "text/html", updateWebpage()); 
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String updateWebpage() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-green-on {background-color: #90ee90;}\n";  // Light green
  ptr +=".button-green-on:active {background-color: #90ee90;}\n";
  ptr +=".button-green-off {background-color: #006400;}\n"; // Dark green
  ptr +=".button-green-off:active {background-color: #006400;}\n";
  ptr +=".button-blue-on {background-color: #add8e6;}\n";   // Light blue
  ptr +=".button-blue-on:active {background-color: #add8e6;}\n";
  ptr +=".button-blue-off {background-color: #00008b;}\n";  // Dark blue
  ptr +=".button-blue-off:active {background-color: #00008b;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Station(STA) Mode</h3>\n";

  ptr +="<p>Green LED: " + String(greenLEDStatus ? "ON" : "OFF") + "</p><a class=\"button button-green-" + String(greenLEDStatus ? "off" : "on") + "\" href=\"/led" + String(greenLEDStatus ? "offgreen" : "ongreen") + "\">" + String(greenLEDStatus ? "OFF" : "ON") + "</a>\n";
  ptr +="<p>Blue LED: " + String(blueLEDStatus ? "ON" : "OFF") + "</p><a class=\"button button-blue-" + String(blueLEDStatus ? "off" : "on") + "\" href=\"/led" + String(blueLEDStatus ? "offblue" : "onblue") + "\">" + String(blueLEDStatus ? "OFF" : "ON") + "</a>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
