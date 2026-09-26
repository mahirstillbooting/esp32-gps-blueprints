#include <TinyGPS++.h>

// =============================
// Pin Configuration
// =============================

#define GPS_RX_PIN 16      // ESP32 RX2 <- GPS TX
#define GPS_TX_PIN 17      // ESP32 TX2 -> GPS RX

#define RED_LED_PIN 21     // Red LED
#define GREEN_LED_PIN 23   // Green LED

#define BUZZER_PIN 22      // Piezo buzzer

#define START_BUTTON 13    // D13 = Start sharing
#define STOP_BUTTON 12     // D12 = Stop sharing


// =============================
// GPS
// =============================

TinyGPSPlus gps;
HardwareSerial GPS_Serial(2);


// =============================
// State
// =============================

bool sharing = false;
bool gpsLocked = false;

bool redLedState = false;

unsigned long lastRedBlink = 0;
unsigned long lastBuzzer = 0;

const unsigned long RED_BLINK_INTERVAL = 500;
const unsigned long BUZZER_INTERVAL = 2000;


// =============================
// Setup
// =============================

void setup() {

  Serial.begin(115200);

  // GPS
  GPS_Serial.begin(
    9600,
    SERIAL_8N1,
    GPS_RX_PIN,
    GPS_TX_PIN
  );

  // LEDs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Buttons
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(STOP_BUTTON, INPUT_PULLUP);


  // Initial state
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println();
  Serial.println("=================================");
  Serial.println(" ESP32 GPS SHARING TEST");
  Serial.println("=================================");
  Serial.println("System started.");
  Serial.println("GPS sharing: OFF");
  Serial.println("Press D13 to START sharing.");
  Serial.println("Press D12 to STOP sharing.");
  Serial.println();
}


// =============================
// Main Loop
// =============================

void loop() {

  // --------------------------------
  // Read GPS data continuously
  // --------------------------------

  while (GPS_Serial.available()) {
    gps.encode(GPS_Serial.read());
  }


  // --------------------------------
  // Check GPS lock
  // --------------------------------

  gpsLocked =
    gps.location.isValid() &&
    gps.satellites.isValid() &&
    gps.satellites.value() >= 4;


  // --------------------------------
  // START BUTTON - D13
  // --------------------------------

  if (digitalRead(START_BUTTON) == LOW) {

    if (!sharing) {

      sharing = true;

      Serial.println();
      Serial.println(">>> GPS SHARING STARTED <<<");

      delay(250);   // simple debounce
    }
  }


  // --------------------------------
  // STOP BUTTON - D12
  // --------------------------------

  if (digitalRead(STOP_BUTTON) == LOW) {

    if (sharing) {

      sharing = false;

      Serial.println();
      Serial.println(">>> GPS SHARING STOPPED <<<");
      Serial.println("GPS location is no longer being shared.");

      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);

      delay(250);   // simple debounce
    }
  }


  // =================================
  // NOT SHARING
  // =================================

  if (!sharing) {

    // Red LED blinks
    if (millis() - lastRedBlink >= RED_BLINK_INTERVAL) {

      lastRedBlink = millis();

      redLedState = !redLedState;

      digitalWrite(RED_LED_PIN, redLedState);
    }

    // Green LED OFF
    digitalWrite(GREEN_LED_PIN, LOW);

    // Buzzer OFF
    noTone(BUZZER_PIN);
  }


  // =================================
  // SHARING + GPS LOCKED
  // =================================

  if (sharing && gpsLocked) {

    // Red LED OFF
    digitalWrite(RED_LED_PIN, LOW);
    redLedState = false;

    // Green LED ON
    digitalWrite(GREEN_LED_PIN, HIGH);


    // --------------------------------
    // Buzzer interval
    // --------------------------------

    if (millis() - lastBuzzer >= BUZZER_INTERVAL) {

      lastBuzzer = millis();

      tone(BUZZER_PIN, 2000, 200);
    }


    // --------------------------------
    // Print GPS coordinates
    // --------------------------------

    if (gps.location.isUpdated()) {

      Serial.println();
      Serial.println("---------- GPS LOCATION ----------");

      Serial.print("Latitude:  ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());

      Serial.print("HDOP: ");
      Serial.println(gps.hdop.hdop(), 2);

      Serial.println("----------------------------------");
    }
  }


  // =================================
  // SHARING BUT GPS NOT LOCKED
  // =================================

  if (sharing && !gpsLocked) {

    // Red LED keeps blinking
    if (millis() - lastRedBlink >= RED_BLINK_INTERVAL) {

      lastRedBlink = millis();

      redLedState = !redLedState;

      digitalWrite(RED_LED_PIN, redLedState);
    }

    // Green OFF
    digitalWrite(GREEN_LED_PIN, LOW);

    // Buzzer OFF
    noTone(BUZZER_PIN);

    static unsigned long lastGPSMessage = 0;

    if (millis() - lastGPSMessage >= 2000) {

      lastGPSMessage = millis();

      Serial.println("Sharing started, but GPS is not locked yet...");

      if (gps.satellites.isValid()) {
        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value());
      }
    }
  }
}