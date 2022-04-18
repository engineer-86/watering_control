#include <Arduino.h>
#include "sensor.hpp"
#include "wifi.hpp"
#include "mqtt.hpp"
void setup()
{
    pinMode(TRIGGER_PIN, INPUT);
    pinMode(ECHO_PIN, OUTPUT);
    Serial.begin(115200);
    connectWithWifi();
    PubSubClient connected_mqtt_client = connectToBroker();
}

void loop()
{
    // int distance = sensor_measurement(TRIGGER_PIN, ECHO_PIN);
    //Serial.println("hello");
}