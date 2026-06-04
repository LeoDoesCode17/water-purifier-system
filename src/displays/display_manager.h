#ifndef DIPLAYS_DISPLAY_MANAGER_H
#define DIPLAYS_DISPLAY_MANAGER_H
#include <Arduino.h>

namespace display_manager {
    void initialize_development();
    void initialize_production();

    void ili9488_print(const String& text);
    void ili9488_print(const char* text);
    void ili9488_print(char c);
    void ili9488_print(int value);
    void ili9488_print(float value, uint8_t decimals = 2);

    void ili9488_println();
    void ili9488_println(const String& text);
    void ili9488_println(const char* text);
    void ili9488_println(char c);
    void ili9488_println(int value);
    void ili9488_println(float value, uint8_t decimals = 2);
}
#endif