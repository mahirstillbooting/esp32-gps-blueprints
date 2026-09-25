/*
  ESP32 DevKit V1 + NEO-6M GPS Module Blueprint
  Repository: mahirstillbooting/esp32-gps-blueprints
  
  Hardware Connections:
  ---------------------
  NEO-6M GPS Module <---> ESP32 DevKit V1
  VCC               <---> 3.3V (or 5V depending on module regulator)
  GND               <---> GND
  TX                <---> RX2 (GPIO 16)
  RX                <---> TX2 (GPIO 17)
  
  Serial Monitor Settings:
  ------------------------
  Baud Rate: 115200
*/

#define RXD2 16
#define TXD2 17

void setup() {
  // Initialize standard Serial interface for debugging output
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("\n--- ESP32 DevKit V1 + NEO-6M GPS Starter ---");
  Serial.println("Initializing Serial2 for GPS communication...");
  
  // Initialize HardwareSerial (Serial2) on GPIO 16 (RX) and GPIO 17 (TX)
  // Default baud rate for NEO-6M GPS module is 9600
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  Serial.println("GPS Serial initialized. Waiting for NMEA sentence stream...");
}

void loop() {
  // Read incoming data from NEO-6M GPS and print to Serial Monitor
  while (Serial2.available() > 0) {
    char c = Serial2.read();
    Serial.print(c);
  }
}
