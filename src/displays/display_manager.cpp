#include "displays/display_manager.h"
#include "displays/tft_ili9488.h"

namespace display_manager
{
    void initialize_development()
    {
        tft_ili9488::initialize();
        tft_ili9488::setTextSize(2);
        tft_ili9488::setTextColor(TFT_GREEN, TFT_BLACK);

        tft_ili9488::println("TFT terminal ready...");
        tft_ili9488::println("ESP32-S3 booting...");
        tft_ili9488::print("WiFi status: ");
        tft_ili9488::println("Disconnected");
    }

    void initialize_production()
    {
        tft_ili9488::initialize_production();
    }

    void ili9488_print(const String &text)
    {
        tft_ili9488::print(text);
    }

    void ili9488_print(const char *text)
    {
        tft_ili9488::print(text);
    }

    void ili9488_print(char c)
    {
        tft_ili9488::print(c);
    }

    void ili9488_print(int value)
    {
        tft_ili9488::print(value);
    }

    void ili9488_print(float value, uint8_t decimals)
    {
        tft_ili9488::print(value, decimals);
    }

    void ili9488_println()
    {
        tft_ili9488::println();
    }

    void ili9488_println(const String &text)
    {
        tft_ili9488::println(text);
    }

    void ili9488_println(const char *text)
    {
        tft_ili9488::println(text);
    }

    void ili9488_println(char c)
    {
        tft_ili9488::println(c);
    }

    void ili9488_println(int value)
    {
        tft_ili9488::println(value);
    }

    void ili9488_println(float value, uint8_t decimals)
    {
        tft_ili9488::println(value, decimals);
    }

    void _draw_labels()
    {
        tft_ili9488::_draw_labels();
    }
    void _update_values(WaterQuality wq)
    {
        tft_ili9488::_update_values(wq);
    }

    void state_idle_ui()
    {
        tft_ili9488::state_idle_ui();
    }

    void state_filling_mixing_tank_ui(float water_level_cm)
    {
        tft_ili9488::state_filling_mixing_tank_ui(water_level_cm);
    }

    void state_raw_sampling_ui(const WaterQuality &wq)
    {
        tft_ili9488::state_raw_sampling_ui(wq);
    }

    void state_ai_measure_ui(const float &moringa_dose_gram, const float &mixing_time_seconds)
    {
        tft_ili9488::state_ai_measure_ui(moringa_dose_gram, mixing_time_seconds);
    }

    void state_dosing_ui(const float &dosing_time_elapsed_s,
                         const float &dosing_time_total_s)
    {
        tft_ili9488::state_dosing_ui(dosing_time_elapsed_s, dosing_time_total_s);
    }

    void state_mixing_ui(const float &mixing_time_elapsed_s,
                         const float &mixing_time_total_s)
    {
        tft_ili9488::state_mixing_ui(mixing_time_elapsed_s, mixing_time_total_s);
    }

    void state_filling_settling_tank_ui(float water_level_cm)
    {
        tft_ili9488::state_filling_settling_tank_ui(water_level_cm);
    }

    void state_settling_ui(const WaterQuality &wq,
                           float elapsed_s,
                           float total_s)
    {
        tft_ili9488::state_settling_ui(wq, elapsed_s, total_s);
    }

    void state_completed_ui()
    {
        tft_ili9488::state_completed_ui();
    }

    void state_failed_ui(const char *reason)
    {
        tft_ili9488::state_failed_ui(reason);
    }
}