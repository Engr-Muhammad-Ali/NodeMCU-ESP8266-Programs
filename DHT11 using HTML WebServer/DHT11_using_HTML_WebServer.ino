#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/* WiFi credentials */
const char* ssid = "SAAD";           // Enter your WiFi SSID
const char* password = "Pakistan";   // Enter your WiFi Password

/* DHT11 sensor setup */
#define DHTPIN D4          // DHT11 connected to D4 pin
#define DHTTYPE DHT11      // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);

/* Data buffers for last hour (assuming data every minute) */
#define DATA_POINTS 60
float temperatureData[DATA_POINTS];
float humidityData[DATA_POINTS];
unsigned long timestamps[DATA_POINTS];
int currentIndex = 0;
bool dataFilled = false;

unsigned long previousMillis = 0;
const long interval = 1000;

void collectData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  unsigned long currentTime = millis();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  temperatureData[currentIndex] = temp;
  humidityData[currentIndex] = hum;
  timestamps[currentIndex] = currentTime;

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  currentIndex++;
  if (currentIndex >= DATA_POINTS) {
    currentIndex = 0;
    dataFilled = true;
  }
}

void setup() {
  Serial.begin(9600);
  delay(100);
  dht.begin();

  pinMode(DHTPIN, INPUT);

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
    temperatureData[i] = 0.0;
    humidityData[i] = 0.0;
    timestamps[i] = 0;
  }

  // Schedule data collection every minute
  collectData(); // Initial data collection
  
  server.on("/", handle_OnConnect);
  server.on("/data", handle_Data);
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

void handle_OnConnect() {
  server.send(200, "text/html", updateWebpage());
}

void handle_Data() {
  // Not used in this implementation but can be used for AJAX if needed
  server.send(200, "text/plain", "Data Endpoint");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String updateWebpage() {
  String ptr = "<!DOCTYPE html><html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "<title>DHT11 Sensor Data</title>\n";
  ptr += "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>\n";
  ptr += "<script type=\"text/javascript\">\n";
  ptr += "google.charts.load('current', {'packages':['gauge','corechart']});\n";
  ptr += "google.charts.setOnLoadCallback(drawChart);\n";
  ptr += "function drawChart() {\n";
  
  /* Temperature Gauge */
  float latestTemp = temperatureData[(currentIndex - 1 + DATA_POINTS) % DATA_POINTS];
  ptr += "var tempData = google.visualization.arrayToDataTable([\n";
  ptr += "['Label', 'Value'],\n";
  ptr += "['Temp', " + String(latestTemp) + "],\n";
  ptr += "]);\n";
  ptr += "var tempOptions = {width: 300, height: 300, redFrom: 40, redTo: 50, yellowFrom:30, yellowTo: 40, minorTicks: 5, max: 50};\n";
  ptr += "var tempChart = new google.visualization.Gauge(document.getElementById('tempGauge'));\n";
  ptr += "tempChart.draw(tempData, tempOptions);\n";

  /* Humidity Gauge */
  float latestHum = humidityData[(currentIndex - 1 + DATA_POINTS) % DATA_POINTS];
  ptr += "var humidityData = google.visualization.arrayToDataTable([\n";
  ptr += "['Label', 'Value'],\n";
  ptr += "['Humidity', " + String(latestHum) + "],\n";
  ptr += "]);\n";
  ptr += "var humidityOptions = {width: 300, height: 300, redFrom: 80, redTo: 100, yellowFrom:60, yellowTo: 80, minorTicks: 5, max: 100};\n";
  ptr += "var humidityChart = new google.visualization.Gauge(document.getElementById('humidityGauge'));\n";
  ptr += "humidityChart.draw(humidityData, humidityOptions);\n";

  /* Temperature Line Chart */
  ptr += "var tempLineData = google.visualization.arrayToDataTable([\n";
  ptr += "['Time', 'Temperature'],\n";
  for(int i = 0; i < DATA_POINTS; i++) {
    int index = (currentIndex + i) % DATA_POINTS;
    if(timestamps[index] == 0) continue; // Skip uninitialized data
    float temp = temperatureData[index];
    // Convert millis() to minutes ago
    unsigned long timeAgo = (millis() - timestamps[index]) / 60000;
    String timeLabel = String(timeAgo) + "m ago";
    ptr += "['" + timeLabel + "', " + String(temp) + "],\n";
  }
  ptr += "]);\n";
  ptr += "var tempLineOptions = {title: 'Temperature (Last Hour)', curveType: 'function', legend: { position: 'bottom' }, height: 300};\n";
  ptr += "var tempLineChart = new google.visualization.LineChart(document.getElementById('tempLineChart'));\n";
  ptr += "tempLineChart.draw(tempLineData, tempLineOptions);\n";

  /* Humidity Line Chart */
  ptr += "var humLineData = google.visualization.arrayToDataTable([\n";
  ptr += "['Time', 'Humidity'],\n";
  for(int i = 0; i < DATA_POINTS; i++) {
    int index = (currentIndex + i) % DATA_POINTS;
    if(timestamps[index] == 0) continue; // Skip uninitialized data
    float hum = humidityData[index];
    // Convert millis() to minutes ago
    unsigned long timeAgo = (millis() - timestamps[index]) / 60000;
    String timeLabel = String(timeAgo) + "m ago";
    ptr += "['" + timeLabel + "', " + String(hum) + "],\n";
  }
  ptr += "]);\n";
  ptr += "var humLineOptions = {title: 'Humidity (Last Hour)', curveType: 'function', legend: { position: 'bottom' }, height: 300};\n";
  ptr += "var humLineChart = new google.visualization.LineChart(document.getElementById('humLineChart'));\n";
  ptr += "humLineChart.draw(humLineData, humLineOptions);\n";

  ptr += "}\n";
  ptr += "setInterval(function() { location.reload(); }, 60000); // Refresh the page every 1 minute\n";
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
  ptr += "<h1>DHT11 Sensor Data</h1>\n";
  ptr += "<div class=\"container\">\n";
  
  /* Temperature Group */
  ptr += "  <div class=\"chart-group\">\n";
  ptr += "    <div id=\"tempGauge\" style=\"width: 300px; height: 300px;\"></div>\n";
  ptr += "    <div id=\"tempLineChart\" style=\"width: 600px; height: 400px;\"></div>\n";
  ptr += "  </div>\n";
  
  /* Humidity Group */
  ptr += "  <div class=\"chart-group\">\n";
  ptr += "    <div id=\"humidityGauge\" style=\"width: 300px; height: 300px;\"></div>\n";
  ptr += "    <div id=\"humLineChart\" style=\"width: 600px; height: 400px;\"></div>\n";
  ptr += "  </div>\n";
  
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;
}
