#include <Arduino.h>
#include <pump.hpp>
#include <sensor.hpp>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <helper.hpp>

SoftwareSerial linkSerial(8, 6); // RX, TX
static int current_water_tank_level = 0;
static bool pump_state = 0;

void setup()
{
  pinMode(ULTRA_SONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_SONIC_ECHO_PIN, INPUT);
  pinMode(PUMP_RELAIS_PIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial)
    continue;

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);
}

void loop()
{
  char out[128];
  // start_pump();
  int moisture_value_1 = analog_moisture_measurement(0);
  int moisture_value_2 = analog_moisture_measurement(1);
  int water_level_value = water_level_measurement(ULTRA_SONIC_TRIGGER_PIN, ULTRA_SONIC_ECHO_PIN);
  print_device_state(moisture_value_1, moisture_value_2, water_level_value);

  // Create the JSON document
  StaticJsonDocument<200> doc;
  
  doc["sensor"]["moisture_sensor_1"] = moisture_value_1;
  doc["sensor"]["moisture_sensor_2"] = moisture_value_2;
  doc["water_level"] = water_level_value;

  // Send the JSON document over the "link" serial port
  serializeJson(doc, linkSerial);

  // Wait
  delay(1000);
}
