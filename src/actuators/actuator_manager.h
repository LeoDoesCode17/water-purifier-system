#ifndef ACTUATORS_ACTUATOR_MANAGER_H
#define ACTUATORS_ACTUATOR_MANAGER_H
namespace actuator_manager {
    void initialize();
    void openServoValve();
    void closeServoValve();
    void openBallValve();
    void closeBallValve();
    void turnOnGearMotor();
    void turnOffGearMotor();
    void turnOnPump();
    void turnOffPump();
}
#endif