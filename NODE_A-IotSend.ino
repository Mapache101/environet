#include <esp_now.h>
#include <WiFi.h>
#include <dhtESP32-rmt.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xCC, 0x50, 0xE3, 0xAF, 0x1A, 0x74};

typedef struct struct_message {
    float temperature;
    float humidity;
    int soilMoisture;
    int lightIntensity;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

int soilSensorPin = 34;     
int lightSensorPin = 36;   

void setup() {
    Serial.begin(115200);
    delay(1000); // Wait a bit to stabilize
  
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    delay(100);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
  
    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 1;  
    peerInfo.encrypt = false;
  
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
    Serial.println("ESP-NOW Initialized. Ready to send.");
}

void loop() {
    // Read sensors
    measureSoilMoisture();
    measureLightIntensity();
    readDHT();
  
    Serial.println("Attempting to send data...");
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
    delay(5000);  // 5 second delay between sends
}

void measureSoilMoisture() {
    myData.soilMoisture = analogRead(soilSensorPin);
}

void measureLightIntensity() {
    myData.lightIntensity = analogRead(lightSensorPin);
}

void readDHT() {
    uint8_t error = read_dht(myData.temperature, myData.humidity, 13, DHT22, 0);
    if(error) {
        Serial.println(error);
    } else {
        Serial.println("DHT22 Sensor Alive");
    }
}
