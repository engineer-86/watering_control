#include <Arduino.h>
#include <sensor.hpp>
#include <wifi.hpp>
#include <mqtt.hpp>

static PubSubClient connected_mqtt_client;
char payload[20];

void setup()
{
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(115200);
    connectWithWifi();
    connected_mqtt_client = connectToBroker();
}

void loop()
{
    float distance_cm = sensor_measurement(TRIGGER_PIN, ECHO_PIN);
    sprintf(payload, "%f", distance_cm);
    connected_mqtt_client.publish("tele/irrigation/state", payload);
}