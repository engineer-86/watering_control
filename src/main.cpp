#include <Arduino.h>
#include <sensor.hpp>
#include <wifi.hpp>
#include <mqtt.hpp>
#include <pump.hpp>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <string.h>

static PubSubClient connected_mqtt_client;
char payload[20];
static int current_water_tank_level = 0;
bool pump_on = false;
bool start_pump_cmd_extern = false;
bool stop_pump_cmd_extern = false;

//WiFiUDP ntpUDP;
//const long utcOffsetInSeconds = 2 * 3600; // UTC+2 Berlin
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);
static StaticJsonDocument<256> info;
char out[128];
//static String info_text = "Booting device";

void setup()
{
    pinMode(0, OUTPUT);
    digitalWrite(0, HIGH);
    pinMode(ULTRA_SONIC_TRIGGER_PIN, OUTPUT);
    pinMode(ULTRA_SONIC_ECHO_PIN, INPUT);
    pinMode(PUMP_RELAIS_PIN, OUTPUT);
    pinMode(MOISTRUE_SENSOR_PIN_DIGITAL, INPUT);
    Serial.begin(115200);
    connectTohWifi();
    connected_mqtt_client = connectToBroker();
    //timeClient.begin();
}

void loop()
{   String info_text = "";
    // timeClient.update();
    // String current_time = timeClient.getFormattedTime();
    // Serial.println(current_time);
    // int hours = timeClient.getHours();
    float distance_cm = water_level_measurement(ULTRA_SONIC_TRIGGER_PIN, ULTRA_SONIC_ECHO_PIN);
    int moisture_value = digital_moisture_measurement(MOISTRUE_SENSOR_PIN_DIGITAL);
    //int moisture_value = analog_moisture_measurement(MOISTRUE_SENSOR_PIN_ANALOG);
    current_water_tank_level = distance_cm;
    info["water_tank_level"] = distance_cm;
    info["moisture"] = moisture_value;
    info["pump on"] = pump_on;
    info["text"] = "info_text";

    // sprintf(payload, "%.2f", distance_cm);

    serializeJson(info, out);
    connected_mqtt_client.publish("tele/irrigation/state", out);

    if (current_water_tank_level > WATER_TANK_LEVEL_MIN)
    {
        info_text = "Water level low, refill!";
        Serial.println(info_text);
        pump_on = false;
        stop_pump();
    }
    else if (current_water_tank_level < WATER_TANK_LEVEL_MAX)
    {
        info_text = "Water level too high, drain!";
        Serial.println(info_text);
    }
    else
    {

        if (((moisture_value == SOIL_DRY) && (!pump_on)) || ((start_pump_cmd_extern == true)))
        {
            pump_on = true;
            start_pump();
            info_text = "Start pump";
            start_pump_cmd_extern = false;
        }
        else if (((moisture_value == SOIL_WET) && (pump_on)) || ((stop_pump_cmd_extern == true)))
        {
            pump_on = false;
            stop_pump();
            info_text = "stop pump";
            stop_pump_cmd_extern = false;
        }
        else
        {
            ;
        }
    }
}