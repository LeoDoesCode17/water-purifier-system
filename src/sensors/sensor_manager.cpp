#include "sensors/sensor_manager.h"
#include "sensors/ultrasonic.h"
#include "sensors/ds18b20.h"
#include "sensors/ads1115.h"
#include "config/pins.h"
#include <Arduino.h>

static float SPEED_SOUND_CM_PER_MICROSECONDS = 0.0343; 
static float ph_calibration = 20.0; 
static float tds_factor = 0.5;
namespace sensor_manager {
    void initialize_all() {
        ultrasonic::initialize();
        bool is_ds18b20_init_success = ds18b20::initialize();
        if (!is_ds18b20_init_success) {
            Serial.println("Failed to initialize ds18b20 sensors");
        }
        bool is_ads1115_init_success = ads1115::initialize();
        if (!is_ads1115_init_success) {
            Serial.println("Failed to inialize ads1115 adc module");
        }
    }

    float get_distance_ultrasonic_1() {
        // clear the trigger
        ultrasonic::trigger_sensor_1_low();
        delayMicroseconds(2);

        // trigger sensor
        ultrasonic::trigger_sensor_1_high();
        delayMicroseconds(10);
        ultrasonic::trigger_sensor_1_low();

        long duration_echo_high = pulseIn(pins::ECHO_1_PIN, HIGH);
        float distance = (duration_echo_high * SPEED_SOUND_CM_PER_MICROSECONDS)/2;
        return distance;
    }

    float get_distance_ultrasonic_2() {
        // clear the trigger
        ultrasonic::trigger_sensor_2_low();
        delayMicroseconds(2);

        // trigger sensor
        ultrasonic::trigger_sensor_2_high();
        delayMicroseconds(10);
        ultrasonic::trigger_sensor_2_low();

        long duration_echo_high = pulseIn(pins::ECHO_2_PIN, HIGH);
        float distance = (duration_echo_high * SPEED_SOUND_CM_PER_MICROSECONDS)/2;
        return distance;
    }

    void update_all() {
        ds18b20::update();
    }

    float get_temperature_c_1() {
        return ds18b20::get_temperature_c_1();
    }

    float get_temperature_c_2() {
        return ds18b20::get_temperature_c_2();
    }

    float get_ph_value_1() {
        float voltage_value = ads1115::get_voltage_value_1(1);
        float ph = 3.5 * voltage_value + ph_calibration;
        return ph;
    }

    float get_ph_value_2() {
        float voltage_value = ads1115::get_voltage_value_2(1);
        float ph = 3.5 * voltage_value + ph_calibration;
        return ph;
    }

    float get_turbidity_ntu_value_1() {
        float voltage_value = ads1115::get_voltage_value_1(0);
        float rough_turbidity_ntu = map(voltage_value, 0, 3.3, 3000, 0); // rough approx
        return rough_turbidity_ntu;
    }

    float get_turbidity_ntu_value_2() {
        float voltage_value = ads1115::get_voltage_value_2(0);
        float rough_turbidity_ntu = map(voltage_value, 0, 3.3, 3000, 0); // rough approx
        return rough_turbidity_ntu;
    }

    float get_tds_ppm_value_1() {
        float voltage_value = ads1115::get_voltage_value_1(2);
        float rough_ppm_value = voltage_value;
        return rough_ppm_value; 
    }

    float get_tds_ppm_value_2() {
        float voltage_value = ads1115::get_voltage_value_2(2);
        float rough_ppm_value = voltage_value;
        return rough_ppm_value; 
    }
}