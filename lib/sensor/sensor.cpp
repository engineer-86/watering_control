#include <Arduino.h>
#include <sensor.hpp>

float sensor_measurement(int trigger_pin, int echo_pin)
{
  long duration;
  float distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * SOUND_VELOCITY / 2;
  Serial.println(distance);
  delay(5000);
  return distance;
}