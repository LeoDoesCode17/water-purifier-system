#include "actuators/gear_motor.h"
#include "config/pins.h"
#include <Arduino.h>
namespace gear_motor {
    void initialize() {
        pinMode(pins::GEAR_MOTOR_PIN, OUTPUT);
    }
    void on() {
        digitalWrite(pins::GEAR_MOTOR_PIN, HIGH);
    }
    void off() {
        digitalWrite(pins::GEAR_MOTOR_PIN, LOW);
    }
}