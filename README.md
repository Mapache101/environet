# environet
iot network using multiple esp nodes, sensors connected with ESPNOW and Wifi to thingspeak 


Hardware:

Node A (ESP32): Send data using ESP-NOW

Node B (ESP32 with an OLED display): Receive data using ESP-NOW and display it on the OLED screen. sends via RX serial to Node C

Node C (ESP): Reads serial TX and uploads to Thingspak

Sensors: DHT22 (Temperature and Humidity), Soil Moisture, and Light Intensity.

Functionality:

Node A should read sensor data and send it to Node B.

Node B should receive the data and display it on the OLED screen.

Node C Reads serial TX and uploads to Thingspak

Error Handling:

Indicate failed sensor readings or unsuccessful data transmissions.

Other Specifics:

The calibration of the sensors will be manual.


Data structure (struct_message) to include character data, an integer, a float, and a boolean.

Node B should also print data received to the Serial Monitor.

Integrate readings from soilMoisture and lightIntensity in Node A's loop function.

Node B's OLED configuration provided.

ThingSpeak integration on Node A to send data to the cloud platform.

From our development:

We have created the Node A code to read from the sensors and send data via ESP-NOW.

Node B successfully receives the data and displays it on the OLED.

We've incorporated error handling and status indicators.

ThingSpeak integration has been achieved.
