 #include <helper.hpp>

  void print_device_state(int moisture_value_1,int moisture_value_2,int water_level_value)
  {
    Serial.print("moisture sensor 1: ");
    Serial.println(moisture_value_1);
    Serial.print("moisture sensor 2: ");
    Serial.println(moisture_value_2);
    Serial.print("water_level_value: ");
    Serial.println(water_level_value);
  }