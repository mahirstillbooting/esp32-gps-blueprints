#include <TinyGPS++.h>

#define BUZZER_PIN 22
#define RED_LED 19
#define BLUE_LED 2

#define GPS_RX 16
#define GPS_TX 17

TinyGPSPlus gps;
HardwareSerial GPS(2);

unsigned long previousMillis = 0;
bool alertState = false;

void setup()
{
    Serial.begin(115200);

    // GPS UART2
    GPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    Serial.println("GPS Test Started...");
}

void loop()
{
    // Read GPS data
    while (GPS.available())
    {
        gps.encode(GPS.read());
    }

    // Check if GPS has a valid location
    if (gps.location.isValid())
    {
        // Blue LED ON = GPS locked
        digitalWrite(BLUE_LED, HIGH);

        // Blink red LED + buzzer
        if (millis() - previousMillis >= 500)
        {
            previousMillis = millis();

            alertState = !alertState;

            if (alertState)
            {
                digitalWrite(RED_LED, HIGH);
                tone(BUZZER_PIN, 2000);
            }
            else
            {
                digitalWrite(RED_LED, LOW);
                noTone(BUZZER_PIN);
            }
        }

        // Display GPS information
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);

        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);

        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value());

        Serial.println("------------------------");

        delay(1000);
    }
    else
    {
        // No GPS lock
        digitalWrite(RED_LED, LOW);
        noTone(BUZZER_PIN);

        // Blue LED blinks slowly
        if (millis() - previousMillis >= 1000)
        {
            previousMillis = millis();

            alertState = !alertState;
            digitalWrite(BLUE_LED, alertState);
        }

        Serial.println("Waiting for GPS satellite lock...");
        delay(500);
    }
}