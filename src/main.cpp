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
SoftwareSerial linkSerial(13, 15); // RX, TX

char out[128];

void setup()
{
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
    if (linkSerial.available())
    {
        StaticJsonDocument<300> doc;
        // Allocate the JSON document
        // This one must be bigger than for the sender because it must store the strings

        // Read the JSON document from the "link" serial port
        Serial.setTimeout(5000);
        DeserializationError err = deserializeJson(doc, linkSerial);

        if (err == DeserializationError::Ok)
        {
            Serial.println(out);
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
        
        serializeJson(doc, out);

        connected_mqtt_client.publish("tele/watering/state", out);
        delay(1000);
    }
}

  // float distance_cm = water_level_measurement(ULTRA_SONIC_TRIGGER_PIN, ULTRA_SONIC_ECHO_PIN);

  // if (current_water_tank_level > WATER_TANK_LEVEL_MIN)
  // {
  //   info_text = "Water level low, refill!";
  //   Serial.println(info_text);
  //   pump_on = false;
  //   stop_pump();
  // }
  // else if (current_water_tank_level < WATER_TANK_LEVEL_MAX)
  // {
  //   info_text = "Water level too high, drain!";
  //   Serial.println(info_text);
  // }
  // else
  // {

  //   if (((moisture_value == SOIL_DRY) && (!pump_on)) || ((start_pump_cmd_extern == true)))
  //   {
  //     pump_on = true;
  //     start_pump();
  //     info_text = "Start pump";
  //     start_pump_cmd_extern = false;
  //   }
  //   else if (((moisture_value == SOIL_WET) && (pump_on)) || ((stop_pump_cmd_extern == true)))
  //   {
  //     pump_on = false;
  //     stop_pump();
  //     info_text = "stop pump";
  //     stop_pump_cmd_extern = false;
  //   }
  //   else
  //   {
  //     ;
  //   }
  // }