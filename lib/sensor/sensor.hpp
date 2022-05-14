#ifndef SENSOR_H
#define SENSOR_H

#define ECHO_PIN 14    // Echo Pin D4
#define TRIGGER_PIN 12 // Trigger Pin D3
#define SOUND_VELOCITY 0.034
#define MOISTRUE_SENSOR_PIN_ANALOG A0
#define MOISTRUE_SENSOR_PIN_DIGITAL 5
#define MOISTRUE_SENSOR_AIR_VALUE 620
#define MOISTRUE_SENSOR_WATER_VALUE 315

#define SOIL_WET 0
#define SOIL_DRY 1

float water_level_measurement(int trigger_pin, int echo_pin);
int analog_moisture_measurement(int sensor_pin);
int digital_moisture_measurement(int sensor_pin);
#endif /* SENSOR_H */