#ifndef SENSOR_H
#define SENSOR_H

#define ECHO_PIN D7    // Echo Pin
#define TRIGGER_PIN D6 // Trigger Pin

int sensor_measurement(int trigger_pin, int echo_pin);

#endif /* SENSOR_H */