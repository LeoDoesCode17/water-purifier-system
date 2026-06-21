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

    void _draw_labels();
    void _update_values(WaterQuality wq);

    void draw_header(const char *title = "Smart Water Purifier");
    void draw_footer(const char *status = "READY");

    // ── Body: one function per state ──────────────────────────────────────
    void state_idle_ui();
    void state_filling_mixing_tank_ui(float water_level_cm);
    void state_raw_sampling_ui(const WaterQuality &wq);
    void state_ai_measure_ui(const float &moringa_dose_gram, const float &mixing_time_seconds);
    void state_dosing_ui(const float &dosing_time_elapsed_s,
                         const float &dosing_time_total_s);
    void state_mixing_ui(const float &mixing_time_elapsed_s,
                         const float &mixing_time_total_s);
    void state_filling_settling_tank_ui(float water_level_cm);
    void state_settling_ui(const WaterQuality &wq,
                           float elapsed_s,
                           float total_s);

    // ── Helpers ───────────────────────────────────────────────────────────
    void clear_body();
    void update_footer_status(const char *status);
    void update_header_time(const char *time_str); // optional clock

    void _draw_centered_text(const char *text, int y, uint16_t color, uint8_t font);
    void _draw_badge(int x, int y, int w, int h, uint16_t bg, uint16_t fg,
                     const char *label, uint8_t font);

}

#endif