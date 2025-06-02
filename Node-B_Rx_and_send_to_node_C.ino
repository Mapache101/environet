#include <esp_now.h>
#include <WiFi.h>
#include "SSD1306.h"

typedef struct struct_message {
  float temperature;
  float humidity;
  int soilMoisture;
  int lightIntensity;
} struct_message;

struct_message myData;
SSD1306 screen(0x3c, 5, 4);

HardwareSerial MySerial(2);  // Use UART2

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  displayData();
  MySerial.write((uint8_t*)&myData, sizeof(myData));  // Send data to Node C
}

void setup() {
  Serial.begin(115200);
  MySerial.begin(115200, SERIAL_8N1, 13, 15);  // RX on GPIO13, TX on GPIO15

  WiFi.mode(WIFI_STA);
  delay(100);

  screen.init();
  screen.setFont(ArialMT_Plain_10);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Initialized. Ready to receive.");
}

void loop() {
  if (MySerial.available()) {
    String message = MySerial.readString();
    if (message == "Uploaded") {
      screen.drawString(0, 48, "Upload Complete");
      screen.display();
      delay(5000);
      displayData();  // Refresh the display after showing the message
    }
  }
}

void displayData() {
  screen.clear();
  screen.setTextAlignment(TEXT_ALIGN_LEFT);
  screen.drawString(0, 0, "Temp: " + String(myData.temperature) + "°C");
  screen.drawString(0, 12, "Humidity: " + String(myData.humidity) + "%");
  screen.drawString(0, 24, "Soil Moisture: " + String(myData.soilMoisture) + "%");
  screen.drawString(0, 36, "Light: " + String(myData.lightIntensity));
  screen.display();
}
