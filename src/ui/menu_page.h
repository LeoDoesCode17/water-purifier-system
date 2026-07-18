#pragma once

#include "page.h"
#include <vector>
#include <String.h>
#include <Arduino.h>

class MenuPage : public Page
{
public:
    void render() override;
    void on_encoder_move(long delta) override;
    void on_button_pressed() override;
    void on_enter() override;

private:
    std::vector<String> _items = {"Start Process", "Collect Data", "Testing Actuator"};
    int _selected_index = 0;
};