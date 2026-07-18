#pragma once
#include "page.h"
#include <vector>
#include <String.h>
#include <Arduino.h>

class TestingActuatorPage : public Page
{
public:
    void render() override;
    void on_encoder_move(long delta) override;
    void on_button_pressed() override;
    void on_enter() override;

private:
    std::vector<String> _items = {"Back to menu", "Pump 1", "Pump 2", "Servo Valve", "Motor Gear"};
    int _selected_index = 0;

    bool _pump1_on = false;
    bool _pump2_on = false;
    bool _servo_valve_on = false;
    bool _motor_gear_on = false;

    void apply_actuator_outputs();
    void apply_pump1();
    void apply_pump2();
    void apply_servo_valve();
    void apply_motor_gear();
};