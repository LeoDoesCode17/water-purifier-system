#include "actuators/actuator_manager.h"
#include "actuators/servo_valve.h"
#include "actuators/ball_valve.h"
#include "actuators/gear_motor.h"
#include "actuators/pump.h"

namespace actuator_manager {
    void initialize() {
        servo_valve::initialize();
        ball_valve::initialize();
        gear_motor::initialize();
        pump::initialize();
    }

    void openServoValve() {
        servo_valve::open();
    }

    void closeServoValve() {
        servo_valve::close();
    }

    void openBallValve() {
        ball_valve::open();
    }

    void closeBallValve() {
        ball_valve::close();
    }

    void turnOnGearMotor() {
        gear_motor::on();
    }

    void turnOffGearMotor() {
        gear_motor::off();
    }

    void turnOnPump() {
        pump::on();
    }

    void turnOffPump() {
        pump::off();
    }
}