#include <WiFi.h>
#include <DHT.h>
#include <WebServer.h>


char ssid[] = "Galaxy A52s 5GF184";
char pass[] = "12345678";


#define DPIN 32      
#define DTYPE DHT11  
#define fire 25
#define MQ135_SENSOR_PIN 35
#define IN1 4
#define IN2 5
#define IN3 13
#define IN4 12


// Use variables instead of #define for IR sensors
int centerIRPin = 18;  // Assign pin number
int leftIRPin = 19;    // Assign pin number
int rightIRPin = 21;   // Assign pin number


int centerIR;
int leftIR;
int rightIR;


int sensitivity = 200;
int leftFlag = 0;
int centerFlag = 0;
int rightFlag = 0;


DHT dht(DPIN, DTYPE);
WiFiServer server(80);


// Timing variables
unsigned long previousMillis = 0;
const long interval = 1000;


void setup() {
  Serial.begin(115200);


  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  // Start the server
  server.begin();
  Serial.println("Web server started.");


  // Initialize pins
  pinMode(fire, INPUT);
  pinMode(MQ135_SENSOR_PIN, INPUT);
  dht.begin();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(centerIRPin, INPUT);  // Use the correct variable
  pinMode(leftIRPin, INPUT);    // Use the correct variable
  pinMode(rightIRPin, INPUT);   // Use the correct variable
}


String interpret_air_quality(int sensor_value) {
  if (sensor_value < 350)
    return "Excellent";
  else if (sensor_value < 500)
    return "Good";
  else if (sensor_value < 850)
    return "Moderate";
  else if (sensor_value < 1000)
    return "Poor";
  else
    return "Dangerous";
}


String fire_detection(int flame) {
  return (flame == HIGH) ? "Not detected" : "Detected";
}


void reverse() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1000);
}


void deviateleft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1000);
}


void deviateright() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1000);
}


void forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1000);
}


void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(1000);
}


void handleClient(WiFiClient client) {
  Serial.println("New client connected");


  // Process request
  while (!client.available()) {
    delay(1);
  }
  String request = client.readStringUntil('\r');
  client.flush();


  // Fetch sensor data
  float temperatureC = dht.readTemperature(false);
  float temperatureF = dht.readTemperature(true); // Convert to Fahrenheit
  float humidity = dht.readHumidity();
  int gasValue = analogRead(MQ135_SENSOR_PIN);
  String airQualityLabel = interpret_air_quality(gasValue * sensitivity / 1023);
  String fireStatus = fire_detection(digitalRead(fire));


  // Prepare the response for the web page
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='5'/>";
  html += "<title>ESP32 Sensor Data</title></head><style>";
  html += "html {background-color: black;color: white;font-family: sans-serif;text-align: center;}";
  html += "p{color: #B27AFB;font-weight: 200;}";
  html += ".spacer {margin: 5%;}";
  html += ".square {height: 100px;width: 100px;background-color: #000;border: #8957ca solid 5px;outline: 5px solid #B27AFB;margin: 5px;border-radius: 15%;margin-inline: auto;}";
  html += ".eye{height: 45px;width: 28px;margin-left: 15px;background-color: #c4a1f1;border-radius: 5px;}</style>";
  html += "<body><div class='spacer'></div>";
  html += "<div class='square'><div>";
  html += "\n\t\t&nbsp;\n\t<div class='eye'></div>";
  html += "\n\t\t<div class='eye' style='margin-left: 55px; margin-top: -45px;'></div>\n\t</div>\n</div>\n";
  html += "<h1>Nexus</h1><br>";


  // Display sensor data
  html += "<h3>Temperature:</h3><p>" + String(temperatureC) + "&deg;C (" + String(temperatureF) + "&deg;F)</p>";
  html += "<h3>Humidity:</h3><p>" + String(humidity) + "%</p>";
  html += "<h3>Air Quality:</h3><p>" + String(gasValue * sensitivity / 1023) + " (" + airQualityLabel + ")</p>";
  html += "<h3>Fire Status:</h3><p>" + fireStatus + "</p>";
  html += "</body></html>";


  // Send response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println(html);
  client.println();
}


void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    // Handle client connection separately, avoiding delays
    handleClient(client);
  }


  // Read IR sensor values
  centerIR = digitalRead(centerIRPin);
  leftIR = digitalRead(leftIRPin);
  rightIR = digitalRead(rightIRPin);


  if (leftIR == 0) {
    leftFlag = 1;
  } else {
    leftFlag = 0;
  }


  if (centerIR == 0) {
    centerFlag = 1;
  } else {
    centerFlag = 0;
  }


  if (rightIR == 0) {
    rightFlag = 1;
  } else {
    rightFlag = 0;
  }


  if (leftFlag == 0 && centerFlag == 0 && rightFlag == 0) {
    forward();
  } else if (centerFlag == 1) {
    if (leftFlag == 1) {
      deviateright();
    } else if (rightFlag == 1) {
      deviateleft();
    } else {
      reverse();
      deviateleft();
    }
  } else if (leftFlag == 1) {
    deviateright();
  } else if (rightFlag == 1) {
    deviateleft();
  } else if (leftFlag == 1 && centerFlag == 1 && rightFlag == 1) {
    reverse();
    deviateleft();
  }
}
