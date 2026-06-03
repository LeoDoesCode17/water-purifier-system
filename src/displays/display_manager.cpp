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
        // initialize tft for production
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

    void ili9488_print(float value, uint8_t decimals = 2)
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

    void ili9488_println(float value, uint8_t decimals = 2)
    {
        tft_ili9488::println(value, decimals);
    }

}