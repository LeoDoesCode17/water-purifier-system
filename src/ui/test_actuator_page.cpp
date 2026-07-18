#include "test_actuator_page.h"
#include "displays/_display_manager.h"
#include "page_manager.h"
#include "config/pins.h"

namespace
{
    TestingActuatorPage *active_instance = nullptr;

    String status_of_callback(int index)
    {
        if (active_instance == nullptr)
        {
            return "";
        }
        return "";
    }
}

void TestingActuatorPage::on_enter()
{
    _selected_index = 0;
    apply_actuator_outputs();
}
void TestingActuatorPage::apply_actuator_outputs()
{
    apply_pump1();
    apply_pump2();
    apply_servo_valve();
    apply_motor_gear();
}
void TestingActuatorPage::render()
{
    active_instance = this;
    _display_manager::draw_menu(
        "Testing Actuator",
        _items,
        _selected_index,
        [](int index) -> String
        {
            switch (index)
            {
            case 1:
                return active_instance->_pump1_on ? "ON" : "OFF";
            case 2:
                return active_instance->_pump2_on ? "ON" : "OFF";
            case 3:
                return active_instance->_servo_valve_on ? "ON" : "OFF";
            case 4:
                return active_instance->_motor_gear_on ? "ON" : "OFF";
            default:
                return "";
            }
        });
}
void TestingActuatorPage::on_encoder_move(long delta)
{
    int count = static_cast<int>(_items.size());
    _selected_index = ((_selected_index + static_cast<int>(delta)) % count + count) % count;
}
void TestingActuatorPage::apply_pump1()
{
    Serial.println(_pump1_on ? "PUMP 1 IS ON" : "PUMP 1 IS OFF");
}
void TestingActuatorPage::apply_pump2()
{
    Serial.println(_pump2_on ? "PUMP 2 IS ON" : "PUMP 2 IS OFF");
}
void TestingActuatorPage::apply_servo_valve()
{
    Serial.println(_servo_valve_on ? "SERVO VALVE IS ON" : "SERVO VALVE IS OFF");
}
void TestingActuatorPage::apply_motor_gear()
{
    Serial.println(_motor_gear_on ? "MOTOR GEAR IS ON" : "MOTOR GEAR IS OFF");
}
void TestingActuatorPage::on_button_pressed()
{
    switch (_selected_index)
    {
    case 0: // Back to menu
        page_manager::navigate_to(page_manager::PageType::MENU);
        return;
    case 1:
        _pump1_on = !_pump1_on;
        apply_pump1();
        break;
    case 2:
        _pump2_on = !_pump2_on;
        apply_pump2();
        break;
    case 3:
        _servo_valve_on = !_servo_valve_on;
        apply_servo_valve();
        break;
    case 4:
        _motor_gear_on = !_motor_gear_on;
        apply_motor_gear();
        break;
    }
    apply_actuator_outputs();
}