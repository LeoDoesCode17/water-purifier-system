#include "sensors/ultrasonic.h"
#include <Arduino.h>
#include "config/pins.h"

namespace ultrasonic {
    void initialize() {
        pinMode(TRIG_1_PIN, OUTPUT);
        pinMode(ECHO_1_PIN, INPUT);
        pinMode(TRIG_2_PIN, OUTPUT);
        pinMode(ECHO_2_PIN, INPUT);
    }
    void trigger_sensor_1_high() {
        digitalWrite(TRIG_1_PIN, HIGH);
    }
    void trigger_sensor_2_high() {
        digitalWrite(TRIG_2_PIN, HIGH);
    }
    void trigger_sensor_1_low() {
        digitalWrite(TRIG_1_PIN, LOW);
    }
    void trigger_sensor_2_low() {
        digitalWrite(TRIG_2_PIN, LOW);
    }
    int get_echo_sensor_1() {
        return digitalRead(ECHO_1_PIN);
    }
    int get_echo_sensor_2() {
        return digitalRead(ECHO_2_PIN);
    }
}