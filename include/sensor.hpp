#ifndef SENSOR_H
#define SENSOR_H

#define ECHO_PIN 14    // Echo Pin D4
#define TRIGGER_PIN 12 // Trigger Pin D3
#define SOUND_VELOCITY 0.034

float sensor_measurement(int trigger_pin, int echo_pin);

#endif /* SENSOR_H */