#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define TRIGGER_PIN D3
#define ECHO_PIN D2

/* WiFi credentials */
const char* ssid = "SAAD";           // Enter your WiFi SSID
const char* password = "Pakistan";   // Enter your WiFi Password

ESP8266WebServer server(80);

/* Data buffers for last hour (assuming data every second) */
#define DATA_POINTS 60
float distanceData[DATA_POINTS];
unsigned long timestamps[DATA_POINTS];
int currentIndex = 0;
bool dataFilled = false;

unsigned long previousMillis = 0;
const long interval = 1000; // 1 second

void setup() {
  Serial.begin(9600);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize data buffers
  for(int i = 0; i < DATA_POINTS; i++) {
    distanceData[i] = 0.0;
    timestamps[i] = 0;
  }

  collectData(); // Initial data collection

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    collectData();
  }
}

float getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance_cm = (duration * 0.0343) / 2;

  return distance_cm;
}

void collectData() {
  float distance = getDistance();
  unsigned long currentTime = millis();

  if (distance < 0) {
    Serial.println("Failed to read from Ultrasonic sensor!");
    return;
  }

  distanceData[currentIndex] = distance;
  timestamps[currentIndex] = currentTime;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  currentIndex++;
  if (currentIndex >= DATA_POINTS) {
    currentIndex = 0;
    dataFilled = true;
  }
}

void handle_OnConnect() {
  server.send(200, "text/html", updateWebpage());
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String updateWebpage() {
  String ptr = "<!DOCTYPE html><html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "<title>Ultrasonic Sensor Data</title>\n";
  ptr += "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>\n";
  ptr += "<script type=\"text/javascript\">\n";
  ptr += "google.charts.load('current', {'packages':['gauge','corechart']});\n";
  ptr += "google.charts.setOnLoadCallback(drawChart);\n";
  ptr += "function drawChart() {\n";

  /* Distance Gauge */
  float latestDistance = distanceData[(currentIndex - 1 + DATA_POINTS) % DATA_POINTS];
  ptr += "var distanceData = google.visualization.arrayToDataTable([\n";
  ptr += "['Label', 'Value'],\n";
  ptr += "['Distance', " + String(latestDistance) + "],\n";
  ptr += "]);\n";
  ptr += "var distanceOptions = {width: 300, height: 300, redFrom: 150, redTo: 200, yellowFrom:100, yellowTo: 150, minorTicks: 5, max: 200};\n";
  ptr += "var distanceChart = new google.visualization.Gauge(document.getElementById('distanceGauge'));\n";
  ptr += "distanceChart.draw(distanceData, distanceOptions);\n";

  /* Distance Line Chart */
  ptr += "var distanceLineData = google.visualization.arrayToDataTable([\n";
  ptr += "['Time', 'Distance'],\n";
  for(int i = 0; i < DATA_POINTS; i++) {
    int index = (currentIndex + i) % DATA_POINTS;
    if(timestamps[index] == 0) continue;
    float distance = distanceData[index];
    unsigned long timeAgo = (millis() - timestamps[index]) / 1000;
    String timeLabel = String(timeAgo) + "s ago";
    ptr += "['" + timeLabel + "', " + String(distance) + "],\n";
  }
  ptr += "]);\n";
  ptr += "var distanceLineOptions = {title: 'Distance (Last Minute)', curveType: 'function', legend: { position: 'bottom' }, height: 300};\n";
  ptr += "var distanceLineChart = new google.visualization.LineChart(document.getElementById('distanceLineChart'));\n";
  ptr += "distanceLineChart.draw(distanceLineData, distanceLineOptions);\n";

  ptr += "}\n";
  ptr += "setInterval(function() { location.reload(); }, 1000); // Refresh the page every 1 second\n";
  ptr += "</script>\n";

  /* CSS for Layout */
  ptr += "<style>\n";
  ptr += "body { font-family: Arial, sans-serif; text-align: center; }\n";
  ptr += ".container { display: flex; justify-content: center; align-items: flex-start; flex-wrap: wrap; }\n";
  ptr += ".chart-group { display: flex; flex-direction: column; align-items: center; margin: 20px; }\n";
  ptr += "</style>\n";
  ptr += "</head>\n";

  /* HTML Body */
  ptr += "<body>\n";
  ptr += "<h1>Ultrasonic Sensor Data</h1>\n";
  ptr += "<div class=\"container\">\n";

  /* Distance Group */
  ptr += "  <div class=\"chart-group\">\n";
  ptr += "    <div id=\"distanceLineChart\" style=\"width: 600px; height: 400px;\"></div>\n";
  ptr += "    <div id=\"distanceGauge\" style=\"width: 300px; height: 300px;\"></div>\n";
  ptr += "  </div>\n";

  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;
}
