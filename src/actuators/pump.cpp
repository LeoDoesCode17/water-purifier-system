#include "actuators/pump.h"
#include "config/pins.h"
#include <Arduino.h>

namespace pump {
    void initialize() {
        pinMode(pins::PUMP_1_PIN, OUTPUT);
        pinMode(pins::PUMP_2_PIN, OUTPUT);
    }
    void on_1() {
        digitalWrite(pins::PUMP_1_PIN, HIGH);
    }
    void off_1() {
        digitalWrite(pins::PUMP_1_PIN, LOW);
    }
    void on_2() {
        digitalWrite(pins::PUMP_2_PIN, HIGH);
    }
    void off_2() {
        digitalWrite(pins::PUMP_2_PIN, LOW);
    }
}