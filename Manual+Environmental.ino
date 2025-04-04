#define BLYNK_TEMPLATE_ID "TMPL3Gvbgtfe5"
#define BLYNK_TEMPLATE_NAME "nexus"
#define BLYNK_AUTH_TOKEN "1cJ6O-NGtyLfOOxMxw45RTzO20223OuG"
#include <DHT.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
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
int sensitivity = 200;
DHT dht(DPIN, DTYPE);
void setup() {
Serial.begin(115200);
// Initialize pins
pinMode(fire, INPUT);
pinMode(MQ135_SENSOR_PIN, INPUT);
dht.begin();
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
Blynk.begin(auth, ssid, pass);
}
String interpret_air_quality(int sensor_value) {
if (sensor_value < 350)return "Excellent";
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
if (flame == HIGH)
return "Detected";
else
return "Not Detected";
}
// forward
BLYNK_WRITE(V0) {
int value = param.asInt();
if (value == 1){
forward();
} else {
stop();
}
}
// reverse
BLYNK_WRITE(V1) {
int value = param.asInt();
if (value == 1){
reverse();
} else {
stop();
}
}
// Right
BLYNK_WRITE(V2) {
int value = param.asInt();
if (value == 1){deviateright();
} else {
stop();
}
}
// Left
BLYNK_WRITE(V3) {
int value = param.asInt();
if (value == 1){
deviateleft();
} else {
stop();
}
}
void reverse() {
Serial.println("Backing up");
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);
}
void deviateleft() {
Serial.println("left");
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
}
void deviateright() {
Serial.println("right");
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
}
void forward() {Serial.println("Forward");
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
}
void stop() {
Serial.println("Stop");
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
void relaySensorData() {
// Read sensor data
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();
int gasValue = analogRead(MQ135_SENSOR_PIN);
String airQuality = interpret_air_quality(gasValue * sensitivity /
1023);
int flameValue = digitalRead(fire);
String fireStatus = fire_detection(flameValue);
// Relay data to Serial monitor
Serial.print("Temperature: ");
Serial.print(temperature);
Serial.println(" °C");
Serial.print("Humidity: ");
Serial.print(humidity);
Serial.println(" %");
Serial.print("Air Quality: ");
Serial.println(airQuality);
Serial.print("Fire Status: ");
Serial.println(fireStatus);Serial.println("----------------------------");
}
void loop() {
Blynk.run();
// Relay sensor data every 2 seconds
static unsigned long lastTime = 0;
unsigned long currentTime = millis();
if (currentTime - lastTime > 2000) { // 2-second interval
relaySensorData();
lastTime = currentTime;
}
}
