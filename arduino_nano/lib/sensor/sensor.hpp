#ifndef SENSOR_H
#define SENSOR_H

#define ULTRA_SONIC_ECHO_PIN 11    // Echo Pin D5
#define ULTRA_SONIC_TRIGGER_PIN 12 // Trigger Pin D6
#define SOUND_VELOCITY 0.034
#define MOISTRUE_SENSOR_PIN_ANALOG A0
#define MOISTRUE_SENSOR_PIN_DIGITAL 5
#define MOISTRUE_SENSOR_AIR_VALUE 1050
#define MOISTRUE_SENSOR_WATER_VALUE 190

#define WATER_TANK_LEVEL_MIN 20 // TODO: map max and min value to percent
#define WATER_TANK_LEVEL_MAX 3

#define SOIL_WET 0
#define SOIL_DRY 1

int water_level_measurement(int trigger_pin, int echo_pin);
int analog_moisture_measurement(int sensor_pin);
int digital_moisture_measurement(int sensor_pin);
#endif /* SENSOR_H */