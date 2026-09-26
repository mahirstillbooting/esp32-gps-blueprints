#include <TinyGPS++.h>

#define GPS_RX_PIN 16   // ESP32 RX2 <- GPS TX
#define GPS_TX_PIN 17   // ESP32 TX2 -> GPS RX

#define LED_PIN 21      // External LED
#define BUZZER_PIN 22   // Piezo buzzer

TinyGPSPlus gps;
HardwareSerial GPS_Serial(2);

unsigned long lastLedTime = 0;
unsigned long lastBuzzerTime = 0;

bool ledState = false;

const unsigned long LED_INTERVAL = 500;      // LED blink every 500 ms
const unsigned long BUZZER_INTERVAL = 2000;  // Beep every 2 seconds
const unsigned long BUZZER_DURATION = 200;   // Beep for 200 ms

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // GPS: RX2 = GPIO16, TX2 = GPIO17
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Serial.println("ESP32 GPS Test Starting...");
  Serial.println("Waiting for GPS fix...");
}

void loop() {

  // Read all available GPS data
  while (GPS_Serial.available()) {
    gps.encode(GPS_Serial.read());
  }

  // Check whether GPS has a valid position
  bool gpsLocked =
    gps.location.isValid() &&
    gps.satellites.isValid() &&
    gps.satellites.value() >= 4;

  if (gpsLocked) {

    // -----------------------------
    // Print GPS coordinates
    // -----------------------------
    if (gps.location.isUpdated()) {

      Serial.println();
      Serial.println("===== GPS LOCKED =====");

      Serial.print("Latitude:  ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());

      Serial.print("HDOP: ");
      Serial.println(gps.hdop.hdop(), 2);

      Serial.println("======================");
    }

    // -----------------------------
    // Blink LED
    // -----------------------------
    if (millis() - lastLedTime >= LED_INTERVAL) {
      lastLedTime = millis();

      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }

    // -----------------------------
    // Buzzer beep
    // -----------------------------
    if (millis() - lastBuzzerTime >= BUZZER_INTERVAL) {
      lastBuzzerTime = millis();

      tone(BUZZER_PIN, 2000, BUZZER_DURATION);
    }

  } else {

    // No GPS lock
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);

    // Show waiting message occasionally
    static unsigned long lastWaitingMessage = 0;

    if (millis() - lastWaitingMessage >= 2000) {
      lastWaitingMessage = millis();

      Serial.println("Waiting for GPS fix...");

      if (gps.satellites.isValid()) {
        Serial.print("Satellites detected: ");
        Serial.println(gps.satellites.value());
      }
    }
  }
}