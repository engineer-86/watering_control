#include <Arduino.h>

int sensor_measurement(int trigger_pin, int echo_pin)
{
    long duration, distance;
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);
    duration = pulseIn(echo_pin, HIGH);
    distance = duration / 58.2;
    Serial.println(distance);
    delay(50); // milliseconds
    return (int) distance;
}