#include "sensors/sensor_manager.h"
#include "sensors/ultrasonic.h"
#include "config/pins.h"
#include <Arduino.h>

namespace sensor_manager {
    static float SPEED_SOUND_CM_PER_MICROSECONDS = 0.0343; 
    void initialize_all() {
        ultrasonic::initialize();
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
}