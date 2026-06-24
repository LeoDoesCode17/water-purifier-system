#ifndef DIPLAYS_DISPLAY_MANAGER_H
#define DIPLAYS_DISPLAY_MANAGER_H
#include <Arduino.h>
#include "config/types.h"

namespace display_manager
{
    void initialize_development();
    void initialize_production();

    void ili9488_print(const String &text);
    void ili9488_print(const char *text);
    void ili9488_print(char c);
    void ili9488_print(int value);
    void ili9488_print(float value, uint8_t decimals = 2);

    void ili9488_println();
    void ili9488_println(const String &text);
    void ili9488_println(const char *text);
    void ili9488_println(char c);
    void ili9488_println(int value);
    void ili9488_println(float value, uint8_t decimals = 2);

    // FOR SENSORS CALIBRATION
    void _draw_labels();
    void _update_values(WaterQuality wq);

    // FOR STATE MACHINE DISPLAY
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
    void state_completed_ui();
    void state_failed_ui(const char *reason = "Unknown error");
}
#endif