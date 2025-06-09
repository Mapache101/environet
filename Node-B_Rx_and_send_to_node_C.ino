#include <esp_now.h>
#include <WiFi.h>
#include "SSD1306.h"

// Define the structure for the sensor data
typedef struct struct_message {
  float temperature;
  float humidity;
  int soilMoisture;
  int lightIntensity;
} struct_message;

// Create an instance of the data structure
struct_message myData;

// Initialize the OLED display on I2C address 0x3C, with SDA on GPIO 5 and SCL on GPIO 4
SSD1306 screen(0x3c, 5, 4);

// Use Hardware Serial port 2 for communication with Node C
HardwareSerial MySerial(2);

// Callback function that is executed when ESP-NOW data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Copy the incoming data into our struct
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Update the OLED screen with the new data
  displayData();
  
  // ==================== UPDATED SECTION START ====================
  // Send data to Node C as formatted text strings, each on a new line.
  // This matches the format that Node C's code is expecting.
  MySerial.println("Temp: " + String(myData.temperature));
  MySerial.println("Humidity: " + String(myData.humidity));
  MySerial.println("Soil Moisture: " + String(myData.soilMoisture));
  MySerial.println("Light: " + String(myData.lightIntensity));
  // ===================== UPDATED SECTION END =====================
}

void setup() {
  // Start the built-in serial monitor for debugging
  Serial.begin(115200);

  // Start the serial communication with Node C (UART2)
  // RX is on GPIO 13, TX is on GPIO 15
  MySerial.begin(115200, SERIAL_8N1, 13, 15);

  // Set the device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  delay(100);

  // Initialize the OLED screen
  screen.init();
  screen.setFont(ArialMT_Plain_10);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receiver callback function
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Initialized. Ready to receive.");
}

void loop() {
  // This part of the loop is for receiving confirmation messages back from Node C.
  // It is not required for the main functionality but can be useful.
void loop() {
  if (MySerial.available()) {
    String message = MySerial.readStringUntil('\n');
    message.trim(); // Removes any leading/trailing whitespace and \r
    if (message == "Uploaded") {
      screen.drawString(0, 48, "Upload Complete");
      screen.display();
      delay(5000);
      displayData(); // Refresh the display after showing the message
    }
  }
}

// Function to display sensor data on the OLED screen
void displayData() {
  screen.clear();
  screen.setTextAlignment(TEXT_ALIGN_LEFT);
  screen.drawString(0, 0, "Temp: " + String(myData.temperature) + "°C");
  screen.drawString(0, 12, "Humidity: " + String(myData.humidity) + "%");
  screen.drawString(0, 24, "Soil Moisture: " + String(myData.soilMoisture) + "%");
  screen.drawString(0, 36, "Light: " + String(myData.lightIntensity));
  screen.display();
}
