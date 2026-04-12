#include "sensors/ultrasonic.h"
#include <Arduino.h>
#include "config/pins.h"

namespace ultrasonic {
    void initialize() {
        pinMode(pins::TRIG_1_PIN, OUTPUT);
        pinMode(pins::ECHO_1_PIN, INPUT);
        pinMode(pins::TRIG_2_PIN, OUTPUT);
        pinMode(pins::ECHO_2_PIN, INPUT);
    }
    void trigger_sensor_1_high() {
        digitalWrite(pins::TRIG_1_PIN, HIGH);
    }
    void trigger_sensor_2_high() {
        digitalWrite(pins::TRIG_2_PIN, HIGH);
    }
    void trigger_sensor_1_low() {
        digitalWrite(pins::TRIG_1_PIN, LOW);
    }
    void trigger_sensor_2_low() {
        digitalWrite(pins::TRIG_2_PIN, LOW);
    }
    int get_echo_sensor_1() {
        return digitalRead(pins::ECHO_1_PIN);
    }
    int get_echo_sensor_2() {
        return digitalRead(pins::ECHO_2_PIN);
    }
}