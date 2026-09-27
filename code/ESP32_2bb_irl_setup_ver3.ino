#include <TinyGPS++.h>
#include <WiFi.h>

const char* WIFI_SSID = "put_wifi_name";
const char* WIFI_PASSWORD = "put_wifi_password";

#define START_BUTTON 25
#define STOP_BUTTON 26

#define BUZZER_PIN 22
#define RED_LED 19
#define GREEN_LED 18
#define BLUE_LED 2

#define GPS_RX 16
#define GPS_TX 17

TinyGPSPlus gps;
HardwareSerial GPS(2);

bool systemActive = false;
bool redLedState = false;
bool blueLedState = false;

unsigned long lastGPSDisplay = 0;
unsigned long lastRedBlink = 0;
unsigned long lastBlueBlink = 0;
unsigned long lastPing = 0;

void startSystem()
{
    systemActive = true;

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    Serial.println();
    Serial.println("==============================");
    Serial.println("SYSTEM STARTING");
    Serial.println("==============================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");

    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 30)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi Status: CONNECTED");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
    }
    else
    {
        Serial.println("WiFi Status: DISCONNECTED");
        digitalWrite(GREEN_LED, LOW);
    }

    lastGPSDisplay = 0;

    Serial.println("GPS detection started.");
    Serial.println("==============================");
}

void stopSystem()
{
    systemActive = false;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    noTone(BUZZER_PIN);

    WiFi.disconnect();

    Serial.println();
    Serial.println("==============================");
    Serial.println("SYSTEM STOPPED");
    Serial.println("Location sharing stopped.");
    Serial.println("GPS detection stopped.");
    Serial.println("WiFi disconnected.");
    Serial.println("==============================");
}

void setup()
{
    Serial.begin(115200);

    GPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    pinMode(START_BUTTON, INPUT_PULLUP);
    pinMode(STOP_BUTTON, INPUT_PULLUP);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    Serial.println();
    Serial.println("==============================");
    Serial.println("ESP32 GPS LOCATION SYSTEM");
    Serial.println("==============================");
    Serial.println("Press D25 to START");
    Serial.println("Press D26 to STOP");
    Serial.println("==============================");
}

void loop()
{
    if (digitalRead(START_BUTTON) == LOW)
    {
        delay(50);

        if (digitalRead(START_BUTTON) == LOW)
        {
            if (!systemActive)
            {
                startSystem();
            }

            while (digitalRead(START_BUTTON) == LOW)
            {
                delay(10);
            }
        }
    }

    if (digitalRead(STOP_BUTTON) == LOW)
    {
        delay(50);

        if (digitalRead(STOP_BUTTON) == LOW)
        {
            if (systemActive)
            {
                stopSystem();
            }

            while (digitalRead(STOP_BUTTON) == LOW)
            {
                delay(10);
            }
        }
    }

    if (!systemActive)
    {
        if (millis() - lastRedBlink >= 500)
        {
            lastRedBlink = millis();

            redLedState = !redLedState;

            digitalWrite(RED_LED, redLedState);
        }

        return;
    }

    while (GPS.available())
    {
        gps.encode(GPS.read());
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
    }
    else
    {
        digitalWrite(GREEN_LED, LOW);

        if (millis() - lastBlueBlink >= 500)
        {
            lastBlueBlink = millis();

            blueLedState = !blueLedState;

            digitalWrite(BLUE_LED, blueLedState);
        }
    }

    if (gps.location.isValid())
    {
        digitalWrite(RED_LED, LOW);

        if (millis() - lastGPSDisplay >= 2000)
        {
            lastGPSDisplay = millis();

            Serial.println();
            Serial.println("===== LOCATION UPDATE =====");

            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println("WiFi Status: CONNECTED");

                Serial.print("IP Address: ");
                Serial.println(WiFi.localIP());

                Serial.print("WiFi RSSI: ");
                Serial.print(WiFi.RSSI());
                Serial.println(" dBm");

                if (millis() - lastPing >= 3000)
                {
                    lastPing = millis();

                    unsigned long pingStart = millis();

                    WiFiClient client;

                    if (client.connect("google.com", 80))
                    {
                        unsigned long pingTime = millis() - pingStart;

                        Serial.print("WiFi Ping: ");
                        Serial.print(pingTime);
                        Serial.println(" ms");

                        client.stop();
                    }
                    else
                    {
                        Serial.println("WiFi Ping: Failed");
                    }
                }
            }
            else
            {
                Serial.println("WiFi Status: DISCONNECTED");
            }

            Serial.print("Latitude : ");
            Serial.println(gps.location.lat(), 6);

            Serial.print("Longitude: ");
            Serial.println(gps.location.lng(), 6);

            Serial.print("Satellites: ");
            Serial.println(gps.satellites.value());

            Serial.print("HDOP: ");

            if (gps.hdop.isValid())
            {
                Serial.println(gps.hdop.hdop(), 2);
            }
            else
            {
                Serial.println("N/A");
            }

            Serial.println("===========================");
        }
    }
    else
    {
        digitalWrite(RED_LED, LOW);

        static unsigned long lastNoFixMessage = 0;

        if (millis() - lastNoFixMessage >= 2000)
        {
            lastNoFixMessage = millis();

            Serial.println();
            Serial.println("GPS: Waiting for satellite lock...");

            Serial.print("Satellites: ");
            Serial.println(gps.satellites.value());

            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println("WiFi Status: CONNECTED");
            }
            else
            {
                Serial.println("WiFi Status: DISCONNECTED");
            }
        }
    }
}