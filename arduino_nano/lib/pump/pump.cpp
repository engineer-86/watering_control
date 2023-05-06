#include <Arduino.h>
#include <pump.hpp>

void start_pump()
{
    delay(1000);
    digitalWrite(PUMP_RELAIS_PIN, HIGH);
}

void stop_pump()
{
    delay(1000);
    digitalWrite(PUMP_RELAIS_PIN, LOW);
}