#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "secrets.h"  // Include your secrets file for WiFi and ThingSpeak credentials

typedef struct struct_message {
  float temperature;
  float humidity;
  int soilMoisture;
  int lightIntensity;
} struct_message;

struct_message myData;
WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);  // UART for communication with ESP32

  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(wifiClient);
}

void loop() {
  if (Serial1.available()) {
    Serial1.readBytes((char*)&myData, sizeof(myData));
    sendDataToThingSpeak();
  }
}

void sendDataToThingSpeak() {
  ThingSpeak.setField(1, myData.temperature);
  ThingSpeak.setField(2, myData.humidity);
  ThingSpeak.setField(3, myData.soilMoisture);
  ThingSpeak.setField(4, myData.lightIntensity);

  int httpResponseCode = ThingSpeak.writeFields(SECRET_CH_ID, SECRET_WRITE_APIKEY);
  if (httpResponseCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
    Serial1.println("Uploaded");  // Inform Node B of successful upload
  } else {
    Serial.println("Failed to send data to ThingSpeak.");
  }
}
