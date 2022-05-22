#include <Arduino.h>
#include <wifi.hpp>
#include <mqtt.hpp>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <string.h>
#include <SoftwareSerial.h>
static PubSubClient connected_mqtt_client;
char payload[20];
static StaticJsonDocument<256> info;
SoftwareSerial linkSerial(0, 2); // RX, TX

char out[128];

void setup()
{
    Serial.begin(115200);
    connectTohWifi();
    connected_mqtt_client = connectToBroker();

    // Initialize the "link" serial port
    // Use the lowest possible data rate to reduce error ratio
    linkSerial.begin(4800);
}

void loop()
{
    if (linkSerial.available())
    {
        // Allocate the JSON document
        // This one must be bigger than for the sender because it must store the strings
        StaticJsonDocument<300> doc;

        // Read the JSON document from the "link" serial port
        DeserializationError err = deserializeJson(doc, linkSerial);

        if (err == DeserializationError::Ok)
        {
            // Print the values
            // (we must use as<T>() to resolve the ambiguity)
            Serial.print("timestamp = ");
            Serial.println(doc["timestamp"].as<long>());
            Serial.print("value = ");
            Serial.println(doc["value"].as<int>());
        }
        else
        {
            // Print error to the "debug" serial port
            Serial.print("deserializeJson() returned ");
            Serial.println(err.c_str());

            // Flush all bytes in the "link" serial port buffer
            while (linkSerial.available() > 0)
                linkSerial.read();
        }
    }

    info["water_tank_level"] = "distance_cm";
    info["moisture"] = "moisture_value";
    info["pump on"] = "pump_on";
    info["text"] = "info_text";
    serializeJson(info, out);

    connected_mqtt_client.publish("tele/watering/state", out);
    delay(1000);
}