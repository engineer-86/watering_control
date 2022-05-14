#include <Arduino.h>
#include <sensor.hpp>
#include <string.h>

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

int analog_moisture_measurement(int sensor_pin)
{ 
  int sensor_value = analogRead(sensor_pin);
  Serial.println(sensor_value);
  int moisture_in_percent = map(sensor_value,
                                MOISTRUE_SENSOR_AIR_VALUE,
                                MOISTRUE_SENSOR_WATER_VALUE, 0, 100);
  Serial.println(moisture_in_percent);
  delay(1000);
  return moisture_in_percent;
}

int digital_moisture_measurement(int sensor_pin)
{ 
  int sensor_value = digitalRead(sensor_pin);
  Serial.println("Digital sensor value:");
  Serial.println(sensor_value);
  delay(1000);
  return sensor_value;
}