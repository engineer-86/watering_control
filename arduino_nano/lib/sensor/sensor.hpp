#ifndef SENSOR_H
#define SENSOR_H

#define ULTRA_SONIC_ECHO_PIN 11    // Echo Pin D11 YELLOW
#define ULTRA_SONIC_TRIGGER_PIN 12 // Trigger Pin D12 BLUE
#define SOUND_VELOCITY 0.034
#define MOISTRUE_SENSOR_PIN_ANALOG A0
#define MOISTRUE_SENSOR_PIN_DIGITAL 5
#define MOISTRUE_SENSOR_AIR_VALUE 1050
#define MOISTRUE_SENSOR_WATER_VALUE 190

#define WATER_TANK_LEVEL_MIN 20 // distance to bottom of tank
#define WATER_TANK_LEVEL_MAX 2 // distance from water to sensor by filled tank

#define SOIL_WET 0
#define SOIL_DRY 1

int water_level_measurement(int trigger_pin, int echo_pin);
int analog_moisture_measurement(int sensor_pin);
int digital_moisture_measurement(int sensor_pin);
#endif /* SENSOR_H */