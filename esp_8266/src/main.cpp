#include <Arduino.h>
#include <wifi.hpp>
#include <mqtt.hpp>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <serial.hpp>
#include <pump.hpp>
#define WATER_TANK_LEVEL_PERCENT_MIN 11
#define WATER_TANK_LEVEL_PERCENT_MAX 95
#define SOIL_DRY 3
#define SOIL_WET 85

bool pump_on = false;
bool start_pump_cmd_extern = false;
bool stop_pump_cmd_extern = false;

static PubSubClient connected_mqtt_client;
SoftwareSerial linkSerial(13, 15); // RX, TX

// pump is in water by water tank value: 11

char payload[128];

void setup()
{
    pinMode(PUMP_RELAIS_PIN, OUTPUT);
    Serial.begin(115200);
    while (!Serial)
        continue;
    connectTohWifi();
    connected_mqtt_client = connectToBroker();

    // Initialize the "link" serial port
    // Use the lowest possible data rate to reduce error ratio
    linkSerial.begin(4800);
}

void loop()
{
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    static StaticJsonDocument<1000> doc;

    if (linkSerial.available())
    {
        // Read the JSON document from the "link" serial port
        // Serial.setTimeout(1000);
        DeserializationError error = deserializeJson(doc, linkSerial);
        String info_text = "";
        doc["pump_on"] = pump_on;

        if (error == DeserializationError::Ok)
        {
            Serial.println(payload);
        }
        else
        {
            Serial.print("deserializeJson() returned ");
            Serial.println(error.c_str());
            // Flush all bytes in the "link" serial port buffer
            while (linkSerial.available() > 0)
                linkSerial.read();
        }

        int average_moisture = ((int)doc["sensor"]["moisture_sensor_1"] + (int)doc["sensor"]["moisture_sensor_2"]) / 2;
     
        if ((int)doc["water_level"] <= WATER_TANK_LEVEL_PERCENT_MIN)
        {
            info_text = "Water level low, refill!";
            Serial.println(info_text);
            pump_on = false;
            stop_pump();
        }
        else if ((int)doc["water_level"] >= WATER_TANK_LEVEL_PERCENT_MAX)
        {
            info_text = "Water level too high, drain!";
            Serial.println(info_text);
        }
        else
        {
            if (((average_moisture <= SOIL_DRY) && (!pump_on)) || ((start_pump_cmd_extern == true)))
            {
                pump_on = true;
                start_pump();
                doc["pump_on"] = pump_on;
                start_pump_cmd_extern = false;
            }
            else if (((average_moisture >= SOIL_WET) && (pump_on)) || ((stop_pump_cmd_extern == true)))
            {
                pump_on = false;
                stop_pump();
                doc["pump_on"] = pump_on;
                stop_pump_cmd_extern = false;
            }
            else
            {
                Serial.println("WEIRD STATE"); // TODO this case is dangerous!
            }
        }
        serializeJson(doc, payload);
        connected_mqtt_client.publish("tele/watering/state", payload);
        delay(500);
    }
}