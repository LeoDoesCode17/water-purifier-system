#include "sensors/sensor_manager.h"
#include "sensors/ultrasonic.h"
#include "sensors/ds18b20.h"
#include "sensors/ads1115.h"
#include "config/constants.h"
#include "config/pins.h"
#include <Arduino.h>
#include <math.h>

static float SPEED_SOUND_CM_PER_MICROSECONDS = 0.0343; 
static float ph_calibration = 20.0; 
static float tds_factor = 0.55;
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
        float voltage_value = ads1115::get_voltage_value_1(constant::CH_PH);
        // Test voltage value
        return voltage_value;

        // Get real PH value
        // float ph = 7.0f - ((voltage_value - constant::PH_NEUTRAL_VOLTAGE) / constant::PH_SLOPE);
        // return constrain(ph, 0.0f, 14.0f);
    }

    float get_ph_value_2() {
        float voltage_value = ads1115::get_voltage_value_2(constant::CH_PH);
        // Test voltage value
        return voltage_value;

        // Get real PH value
        // float ph = 7.0f - ((voltage_value - constant::PH_NEUTRAL_VOLTAGE) / constant::PH_SLOPE);
        // return constrain(ph, 0.0f, 14.0f);
    }

    float get_turbidity_ntu_value_1() {
        float voltage_value = ads1115::get_voltage_value_1(constant::CH_TURBIDITY);
        // float rough_turbidity_ntu = -1120.4*pow(voltage_value, 2) + 5742.3*voltage_value - 4352.9;
        // return rough_turbidity_ntu;
        return voltage_value;
    }

    float get_turbidity_ntu_value_2() {
        float voltage_value = ads1115::get_voltage_value_2(constant::CH_TURBIDITY);
        // float rough_turbidity_ntu = -1120.4*pow(voltage_value, 2) + 5742.3*voltage_value - 4352.9;
        // return rough_turbidity_ntu;
        return voltage_value;
    }

    float get_tds_ppm_value_1() {
        float voltage_value = ads1115::get_voltage_value_1(constant::CH_TDS);
        float compensation_coeff = 1.0f + 0.02f*(get_temperature_c_1() - 25.0f);
        float Vc = voltage_value / compensation_coeff;
        float tds = (133.42f * powf(Vc, 3)
                - 255.86f * powf(Vc, 2)
                + 857.39f * Vc) * 0.5f;
        return constrain(tds, 0.0f, 3000.0f);

        // float electrical_conductivity = tds_factor * (133.42*pow(voltage_value, 3) - 255.86*pow(voltage_value, 2) + 857.39*voltage_value);
        // float water_temperature = ds18b20::get_temperature_c_1();
        // float electrical_conductivity_25 = electrical_conductivity / (1 + 0.02*(water_temperature - 25.0));
        // float rough_ppm_value = electrical_conductivity_25 * 0.5;
        // return rough_ppm_value; 
    }

    float get_tds_ppm_value_2() {
        float voltage_value = ads1115::get_voltage_value_2(constant::CH_TDS);
        float compensation_coeff = 1.0f + 0.02f*(get_temperature_c_2() - 25.0f);
        float Vc = voltage_value / compensation_coeff;
        float tds = (133.42f * powf(Vc, 3)
                - 255.86f * powf(Vc, 2)
                + 857.39f * Vc) * 0.5f;
        return constrain(tds, 0.0f, 3000.0f);
        
        // float electrical_conductivity = tds_factor * (133.42*pow(voltage_value, 3) - 255.86*pow(voltage_value, 2) + 857.39*voltage_value);
        // float water_temperature = ds18b20::get_temperature_c_2();
        // float electrical_conductivity_25 = electrical_conductivity / (1 + 0.02*(water_temperature - 25.0));
        // float rough_ppm_value = electrical_conductivity_25 * 0.5;
        // return rough_ppm_value;
    }
}