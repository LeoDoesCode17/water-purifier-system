#ifndef DIPLAYS_TFT_ILI9488_H
#define DIPLAYS_TFT_ILI9488_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config/types.h"

namespace tft_ili9488
{
    void initialize();

    void clear();
    void setTextColor(uint16_t fg, uint16_t bg = TFT_BLACK);
    void setTextSize(uint8_t size);
    void setCursor(uint16_t x, uint16_t y);

    void print(const String& text);
    void print(const char* text);
    void print(char c);
    void print(int value);
    void print(float value, uint8_t decimals = 2);

    void println();
    void println(const String& text);
    void println(const char* text);
    void println(char c);
    void println(int value);
    void println(float value, uint8_t decimals = 2);
}

#endif