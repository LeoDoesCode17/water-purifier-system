#ifndef DISPLAYS_TFT_TERMINAL_H
#define DISPLAYS_TFT_TERMINAL_H
#include <Arduino.h>
#include "config/types.h"

namespace tft_terminal {
    void begin();
    void print(const String& text);
    void println(const String& text = "");
    void draw_table(const WaterQuality& s1, const WaterQuality& s2);
}
#endif
