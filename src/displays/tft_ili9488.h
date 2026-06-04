#ifndef DIPLAYS_TFT_ILI9488_H
#define DIPLAYS_TFT_ILI9488_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config/types.h"

namespace tft_ili9488
{
    void initialize();
    void initialize_production();

    void clear();
    void setTextColor(uint16_t fg, uint16_t bg = TFT_BLACK);
    void setTextSize(uint8_t size);
    void setCursor(uint16_t x, uint16_t y);

    void print(const String &text);
    void print(const char *text);
    void print(char c);
    void print(int value);
    void print(float value, uint8_t decimals = 2);

    void println();
    void println(const String &text);
    void println(const char *text);
    void println(char c);
    void println(int value);
    void println(float value, uint8_t decimals = 2);

    void draw_idle_screen();
    void clear_prod_screen();
    void show_filling_tank_screen(float water_level_cm, float water_level_percent);
    void show_raw_sampling_screen(float ph, float ntu, float ppm, float water_level_cm, float temp_c);
    void show_measure_moringa_screen(float dose_gram);
    void show_dosing_screen(float dose_gram);
    void show_mixing_screen(uint32_t remaining_seconds);
    void show_settling_screen(uint32_t remaining_seconds);
    void show_completed_screen(float final_ph, float final_ntu, float final_ppm);
    void show_failed_screen();

}

#endif