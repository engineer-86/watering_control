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
SoftwareSerial linkSerial(13,15 ); // RX, TX

StaticJsonDocument<300> doc;
char out[300];

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

        // Read the JSON document from the "link" serial port
        DeserializationError err = deserializeJson(doc, linkSerial);

        if (err == DeserializationError::Ok)
        {
            // Print the values
            // (we must use as<T>() to resolve the ambiguity)

            Serial.print("moisture sensor 1 = ");
            Serial.println(doc["moisture_sensor_1"].as<int>());
            Serial.print("moisture sensor 2 = ");
            Serial.println(doc["moisture_sensor_2"].as<int>());
            Serial.print("water level = ");
            Serial.println(doc["water_level"].as<float>());
            Serial.print("pump state = ");
            Serial.println(doc["pump_on"].as<int>());
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

    serializeJson(doc, out);
    connected_mqtt_client.publish("tele/watering/state", out);
    delay(1000);
}