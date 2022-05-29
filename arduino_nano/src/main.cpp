#include <Arduino.h>
#include <pump.hpp>
#include <sensor.hpp>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial linkSerial(7, 4); // RX, TX
static int current_water_tank_level = 0;
static int pump_on = 0;

void setup()
{
  pinMode(ULTRA_SONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_SONIC_ECHO_PIN, INPUT);
  pinMode(PUMP_RELAIS_PIN, OUTPUT);

  Serial.begin(115200);
   while (!Serial) continue;

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);
}

void loop()
{
  char out[128];

  // Values we want to transmit
  bool pump_state = pump_on;
  start_pump();
  int moisture_value_1 = analog_moisture_measurement(0);
  int moisture_value_2 = analog_moisture_measurement(1);
  int water_level_value = water_level_measurement(ULTRA_SONIC_TRIGGER_PIN, ULTRA_SONIC_ECHO_PIN);

  Serial.print("moisture sensor 1: ");
  Serial.println(moisture_value_1);
  Serial.print("moisture sensor 2: ");
  Serial.println(moisture_value_2);
  Serial.print("water_level_value: ");
  Serial.println(water_level_value);
  Serial.print("pump_state: ");
  Serial.println(pump_state);

  // Create the JSON document
  StaticJsonDocument<200> doc;

  doc["sensor"]["moisture_sensor_1"] = moisture_value_1;
  doc["sensor"]["moisture_sensor_2"] = moisture_value_2;
  doc["water_level"] = water_level_value;
  doc["pump_on"] = pump_state;
  // Send the JSON document over the "link" serial port
  serializeJson(doc, linkSerial);

  // Wait
  delay(1000);

  // float distance_cm = water_level_measurement(ULTRA_SONIC_TRIGGER_PIN, ULTRA_SONIC_ECHO_PIN);

  // if (current_water_tank_level > WATER_TANK_LEVEL_MIN)
  // {
  //   info_text = "Water level low, refill!";
  //   Serial.println(info_text);
  //   pump_on = false;
  //   stop_pump();
  // }
  // else if (current_water_tank_level < WATER_TANK_LEVEL_MAX)
  // {
  //   info_text = "Water level too high, drain!";
  //   Serial.println(info_text);
  // }
  // else
  // {

  //   if (((moisture_value == SOIL_DRY) && (!pump_on)) || ((start_pump_cmd_extern == true)))
  //   {
  //     pump_on = true;
  //     start_pump();
  //     info_text = "Start pump";
  //     start_pump_cmd_extern = false;
  //   }
  //   else if (((moisture_value == SOIL_WET) && (pump_on)) || ((stop_pump_cmd_extern == true)))
  //   {
  //     pump_on = false;
  //     stop_pump();
  //     info_text = "stop pump";
  //     stop_pump_cmd_extern = false;
  //   }
  //   else
  //   {
  //     ;
  //   }
  // }
}