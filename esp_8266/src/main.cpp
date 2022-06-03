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
#define WATER_TANK_LEVEL_PERCENT_MIN 11
#define WATER_TANK_LEVEL_PERCENT_MAX 95
#define SOIL_DRY 3
#define SOIL_WET 85
#define SERIAL_RX 13 // 13
#define SERIAL_TX 15 // 15

bool pump_on = false;
bool start_pump_cmd_extern = false;
bool stop_pump_cmd_extern = false;

static PubSubClient connected_mqtt_client;
SoftwareSerial linkSerial(13, 15);

char payload[200];

void setup()
{
    pinMode(PUMP_RELAIS_PIN, OUTPUT);

    connectTohWifi();
    connected_mqtt_client = connectToBroker();

    Serial.begin(115200);
    while (!Serial)
    {
        Serial.println("no serial con");
        continue;
    }
    // Initialize the "link" serial port
    // Use the lowest possible data rate to reduce error ratio
    linkSerial.begin(4800);
}

void loop()
{

    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    static StaticJsonDocument<300> doc;
    static StaticJsonDocument<300> to_send;

    if (linkSerial.available())
    {

        Serial.println("Link to serial connection available");
        // Read the JSON document from the "link" serial port

        DeserializationError err = deserializeJson(doc, linkSerial);
        String info_text = "";
        doc["pump_on"] = pump_on;

        if (err == DeserializationError::Ok)
        {
            to_send = doc;
            // Serial.println(payload);
            Serial.println("Serial Message OK");
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
            // Print error to the "debug" serial port
            Serial.print("deserializeJson() returned ");
            Serial.println(err.c_str());
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
            // else
            // {
            //     Serial.println("WEIRD STATE"); // TODO this case is dangerous!
            // }
        }
        serializeJson(to_send, payload);
        connected_mqtt_client.publish("tele/watering/state", payload);
        delay(500);
    }
    // else
    // {
    //     Serial.println("Link to serial connection not available! Check RX and TX wires!");
    //     delay(1000);
    //     connected_mqtt_client.publish("tele/watering/state", "Link to serial connection not available! Check RX and TX wires!");
    // }
}
