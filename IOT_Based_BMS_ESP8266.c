#include <ESP8266WiFi.h>

// Replace with your WiFi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// ThingSpeak settings
const char* server = "api.thingspeak.com";
String apiKey = "Your_ThingSpeak_API_Key";

WiFiClient client;

const int batteryPin = A0;  // Analog pin to read battery voltage
float batteryVoltage = 0.0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  // Read battery voltage
  int analogValue = analogRead(batteryPin);
  batteryVoltage = (analogValue * 3.3) / 1023 * (5.0); // Adjust if using voltage divider

  Serial.print("Battery Voltage: ");
  Serial.println(batteryVoltage);

  // Connect to ThingSpeak
  if (client.connect(server, 80)) {
    String url = "/update?api_key=" + apiKey + "&field1=" + String(batteryVoltage);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");
    Serial.println("Data sent to ThingSpeak.");
  } else {
    Serial.println("Failed to connect to server.");
  }

  client.stop();
  delay(15000); // Upload data every 15 seconds
}
