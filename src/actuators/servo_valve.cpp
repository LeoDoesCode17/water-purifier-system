#include "servo_valve.h"
#include <ESP32Servo.h>
#include "config/pins.h"

static Servo servoValve;
namespace servo_valve {
    void initialize() {
        servoValve.attach(pins::SERVO_VALVE_PIN);
        servoValve.write(0);
    }

    void open() {
        servoValve.write(180);
    }

    void close() {
        servoValve.write(0);
    }
}