#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN D4     // DHT11 data pin
#define DHTTYPE DHT11 // DHT 11
#define LED_PIN D2    // LED pin
#define RAIN_PIN D1   // rain pin 

const char* ssid = "poonam";  // Replace with your network SSID
const char* password = "poonam23";  // Replace with your network password

unsigned long myChannelNumber = 2617495;  // Replace with your ThingSpeak channel number
const char* myWriteAPIKey = "EE4EEQEDXZOVMTBV";  // Replace with your ThingSpeak write API key

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

float t, h;
int ldrValue, r;
String myStatus = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);

  dht.begin();
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure LED is off initially
  pinMode(RAIN_PIN, INPUT);  // Set RAIN_PIN as input
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  ldrValue = analogRead(A0);
  int RainDetected = digitalRead(RAIN_PIN);
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: "); Serial.print(h);
  Serial.print(" %\tTemperature: "); Serial.print(t);
  Serial.print(" *C\tLDR Value: "); Serial.println(ldrValue);
  Serial.print("\tRain Detected: ");
  Serial.println(RainDetected ? "Yes" : "No");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, ldrValue);
  ThingSpeak.setField(4, RainDetected ? 1 : 0);
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  // Control LED based on LDR value
  if (ldrValue > 500) {  // Adjust the threshold value as needed
    digitalWrite(LED_PIN, HIGH);  // Turn LED on
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn LED off
  }
  
  // Send data to HTTP server
  String body = "{\"temperature\": " + String(t, 2) + ",\"humidity\": " + String(h, 2) + ",\"ldrValue\": " + String(ldrValue) + ",\"RainDetected\": " + String(RainDetected) + "}";
  Serial.println("JSON object: " + body);

  HTTPClient httpClient;
  httpClient.begin(client, "http://192.168.152.9:3000/readingdata");
  httpClient.addHeader("Content-Type", "application/json");

  int status = httpClient.POST(body);

  if (status > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(status);
    String payload = httpClient.getString();
    Serial.println("Response payload: " + payload);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpClient.errorToString(status).c_str());
  }

  httpClient.end();

  delay(1000);  // Wait for 1 second before next update
}
