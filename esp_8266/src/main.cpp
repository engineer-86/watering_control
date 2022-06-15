#include <Arduino.h>
#include <wifi.hpp>
#include <mqtt.hpp>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <pump.hpp>

#define WATER_TANK_LEVEL_PERCENT_MIN 10
#define WATER_TANK_LEVEL_PERCENT_MAX 100
#define SOIL_WET 75
#define SERIAL_RX 13 // 13
#define SERIAL_TX 15 // 15

static String info_text = "Watering system ready";
bool pump_on = false;
static PubSubClient connected_mqtt_client;
SoftwareSerial linkSerial(SERIAL_RX, SERIAL_TX);

char payload[200];

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        Serial.println("No serial connection to arduino");
        continue;
    }

    linkSerial.begin(4800);
    pinMode(PUMP_RELAIS_PIN, OUTPUT);
    void callback();
    connectTohWifi();
    connected_mqtt_client = connectToBroker();
}

void loop()
{

    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    static StaticJsonDocument<300> doc;
    static StaticJsonDocument<300> to_publish;

    while (linkSerial.available())
    {

        Serial.println("Link to serial connection available");
        // Read the JSON document from the "link" serial port

        DeserializationError err = deserializeJson(doc, linkSerial);

        doc["pump_on"] = pump_on;
        doc["info"] = info_text;

        if (err == DeserializationError::Ok)
        {
            to_publish = doc;
            Serial.print("water_level = ");
            Serial.println(doc["water_level"].as<int>());

            Serial.print("moisture_sensor_1 = ");
            Serial.println(doc["moisture_sensor_1"].as<int>());

            Serial.print("moisture_sensor_2 = ");
            Serial.println(doc["water_level"].as<int>());

            Serial.print("pump_on = ");
            Serial.println(doc["pump_on"].as<bool>());
        }
        else
        {
            Serial.print("deserializeJson() returned "); // Print error to the "debug" serial port
            Serial.println(err.c_str());

            while (linkSerial.available() > 0) // Flush all bytes in the "link" serial port buffer
                linkSerial.read();
        }

        if ((int)doc["water_level"] < WATER_TANK_LEVEL_PERCENT_MIN)
        {
            info_text = "Water level low, refill!";
            doc["info"] = info_text;
            Serial.println(info_text);
            pump_on = false;
            stop_pump();
        }
        else if ((int)doc["water_level"] > WATER_TANK_LEVEL_PERCENT_MIN)
        {
            info_text = "Water level good!";
            doc["info"] = info_text;

            if (((int)doc["sensor"]["moisture_sensor_2"] < SOIL_WET) && !pump_on)
            {
                info_text = "Soil dry, and pump2 on";
                pump_on = true;
                start_pump();
                doc["pump_on"] = pump_on;
                doc["info"] = info_text;
            }
            else if (((int)doc["sensor"]["moisture_sensor_2"] > SOIL_WET) && pump_on)
            {
                info_text = "Soil wet and pump2 off";
                pump_on = false;
                stop_pump();
                doc["pump_on"] = pump_on;
                doc["info"] = info_text;
            }
        }

        serializeJson(to_publish, payload);
        connected_mqtt_client.publish("tele/watering/state", payload);
        delay(500);
    }
}
