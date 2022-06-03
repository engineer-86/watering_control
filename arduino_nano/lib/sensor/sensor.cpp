#include <Arduino.h>
#include <sensor.hpp>
#include <string.h>

int water_level_measurement(int trigger_pin, int echo_pin)
{
  long duration;
  int distance;
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  distance = duration * SOUND_VELOCITY / 2;
  delay(500);
  int distance_in_percent = map(distance,
                                WATER_TANK_LEVEL_MIN,
                                WATER_TANK_LEVEL_MAX, 0, 100);
  return distance_in_percent;
}

int analog_moisture_measurement(int sensor_pin)
{ 
  int sensor_value = analogRead(sensor_pin);
  int moisture_in_percent = map(sensor_value,
                                MOISTRUE_SENSOR_AIR_VALUE,
                                MOISTRUE_SENSOR_WATER_VALUE, 0, 100);
  
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