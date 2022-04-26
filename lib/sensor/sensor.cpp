#include <Arduino.h>
#include <sensor.hpp>

float water_level_measurement(int trigger_pin, int echo_pin)
{
  long duration;
  float distance;
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  distance = duration * SOUND_VELOCITY / 2;
  Serial.println(distance);
  delay(1000);
  return distance;
}

float plant_moisture_measurement(int sensor_pin)
{
  float moisture = analogRead(sensor_pin);
  return moisture;
}