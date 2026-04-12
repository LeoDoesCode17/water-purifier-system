#include "sensors/sensor_manager.h"
#include "sensors/ultrasonic.h"
#include "sensors/ds18b20.h"
#include "config/pins.h"
#include <Arduino.h>

static float SPEED_SOUND_CM_PER_MICROSECONDS = 0.0343; 
namespace sensor_manager {
    void initialize_all() {
        ultrasonic::initialize();
        bool is_ds18b20_init_success = ds18b20::initialize();
        if (!is_ds18b20_init_success) {
            Serial.println("Failed to initialize ds18b20 sensors");
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

    float update_all() {
        ds18b20::update();
    }

    float get_temperature_c_1() {
        return ds18b20::get_temperature_c_1();
    }

    float get_temperature_c_2() {
        return ds18b20::get_temperature_c_2();
    }
}