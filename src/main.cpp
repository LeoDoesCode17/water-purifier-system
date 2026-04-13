#include <Arduino.h>
#include "config/pins.h"
#include "config/constants.h"
#include "sensors/sensor_manager.h"
#include "actuators/actuator_manager.h"

void setup() {
  Serial.begin(constant::BAUD_RATE);
  sensor_manager::initialize_all();
  actuator_manager::initialize();
}

void loop() {
  sensor_manager::update_all();
  float distance_1 = sensor_manager::get_distance_ultrasonic_1();
  float distance_2 = sensor_manager::get_distance_ultrasonic_2();
  float temperature_1 = sensor_manager::get_temperature_c_1();
  float temperature_2 = sensor_manager::get_temperature_c_2();
  Serial.println("============================");
  Serial.print("Distance sensor 1: ");
  Serial.println(distance_1);
  Serial.print("Distance sensor 2: ");
  Serial.println(distance_2);
  Serial.print("Temperature sensor 1: ");
  Serial.println(temperature_1);
  Serial.print("Temperature sensor 2: ");
  Serial.println(temperature_2);
  Serial.println("============================");
  delay(200);
}