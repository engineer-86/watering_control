#include <Arduino.h>
#include <sensor.hpp>
#include <wifi.hpp>
#include <mqtt.hpp>
#include <pump.hpp>

static PubSubClient connected_mqtt_client;
char payload[20];
bool pump_on = false;
bool pump_start_command = false;

void setup()
{
    pinMode(0, OUTPUT);
    digitalWrite(0, HIGH);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(PUMP_RELAIS_PIN, OUTPUT);
    pinMode(MOISTRUE_SENSOR_PIN_DIGITAL, INPUT);
    Serial.begin(115200);
    connectTohWifi();
    connected_mqtt_client = connectToBroker();
}

void loop()
{
    // float distance_cm = water_level_measurement(TRIGGER_PIN, ECHO_PIN);
    // sprintf(payload, "%.2f", distance_cm);
    // connected_mqtt_client.publish("tele/irrigation/state", payload);

    // analog_moisture_measurement(MOISTRUE_SENSOR_PIN);
    // digital_moisture_measurement(5);

    if (digital_moisture_measurement(MOISTRUE_SENSOR_PIN_DIGITAL) == SOIL_DRY && !pump_on ||  )
    {
        start_pump();
    }
    else
    {
        stop_pump();
    }
}