#include <Arduino.h>
#include <ArduinoJson.h>  
#include "config/pins.h"
#include "config/constants.h"
#include "config/secrets.h"
#include "sensors/sensor_manager.h"
#include "actuators/actuator_manager.h"
#include "networks/wifi.h"
#include "networks/mqtt.h"
#include "config/types.h"

unsigned long lastPublish = 0;
const unsigned long publishInterval = 2000;
unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 5000;

void setup() {
  Serial.begin(constant::BAUD_RATE);

  sensor_manager::initialize_all();
  actuator_manager::initialize();
  delay(1000);

  wifi::initialize();
  mqtt::initialize();
  mqtt::connect();
}

void loop() {
  if (wifi::is_connected()) {
    Serial.println("[WIFI] Disconnected. Reconnecting...");
    wifi::initialize();
  }

  if (!mqtt::is_connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt >= reconnectInterval) {
      lastReconnectAttempt = now;
      mqtt::connect();
    }
  } else {
    mqtt::loop();
  }

  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;

     sensor_manager::update_all();

    Serial.println("================== BOX SENSORS 1 ==================");

    float distance_1 = sensor_manager::get_distance_ultrasonic_1();
    float temperature_1 = sensor_manager::get_temperature_c_1();
    float ntu_1 = sensor_manager::get_turbidity_ntu_value_1();
    float ph_1 = sensor_manager::get_ph_value_1();
    float ppm_1 = sensor_manager::get_tds_ppm_value_1();

    Serial.print("Distance 1    : ");
    Serial.println(distance_1);
    Serial.print("Temperature 1 : ");
    Serial.println(temperature_1);
    Serial.print("NTU 1         : ");
    Serial.println(ntu_1);
    Serial.print("PH 1          : ");
    Serial.println(ph_1);
    Serial.print("PPM 1         : ");
    Serial.println(ppm_1);

    Serial.println("===================================================");

    WaterQuality water_quality = {
      .ntu=ntu_1, 
      .ph=ph_1, 
      .ppm=ppm_1, 
      .level=distance_1, 
      .temperature=temperature_1
    };
    mqtt::publish(water_quality);
  }
}