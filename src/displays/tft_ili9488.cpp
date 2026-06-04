#include "displays/tft_ili9488.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

static TFT_eSPI _tft = TFT_eSPI();

namespace production
{
    // For production constant display
    constexpr uint16_t COLOR_BG = TFT_BLACK;
    constexpr uint16_t COLOR_CARD = 0x18E3;
    constexpr uint16_t COLOR_TITLE_BAR = TFT_DARKCYAN;
    constexpr uint16_t COLOR_TEXT = TFT_WHITE;
    constexpr uint16_t COLOR_LABEL = TFT_CYAN;
    constexpr uint16_t COLOR_VALUE = TFT_YELLOW;
    constexpr uint16_t COLOR_SUCCESS = TFT_GREEN;
    constexpr uint16_t COLOR_WARNING = TFT_ORANGE;
    constexpr uint16_t COLOR_ERROR = TFT_RED;
    // End production contant display

    String formatTimeHHMMSS(uint32_t totalSeconds)
    {
        uint32_t hours = totalSeconds / 3600;
        uint32_t minutes = (totalSeconds % 3600) / 60;
        uint32_t seconds = totalSeconds % 60;

        char buffer[16];
        sprintf(buffer, "%02lu:%02lu:%02lu", hours, minutes, seconds);
        return String(buffer);
    }

    void drawBaseLayout(const String &title, uint16_t titleColor = COLOR_TITLE_BAR)
    {
        _tft.fillScreen(COLOR_BG);

        _tft.fillRoundRect(10, 10, 460, 50, 8, titleColor);
        _tft.fillRoundRect(10, 70, 460, 210, 8, COLOR_CARD);
        _tft.fillRoundRect(10, 290, 460, 20, 6, TFT_DARKGREY);

        _tft.setTextDatum(MC_DATUM);
        _tft.setTextColor(COLOR_TEXT, titleColor);
        _tft.setTextSize(2);
        _tft.drawString(title, 240, 35);

        _tft.setTextDatum(TL_DATUM);
        _tft.setTextColor(COLOR_TEXT, COLOR_CARD);
        _tft.setTextSize(2);
    }

    void drawMessage(const String &message, int16_t x = 25, int16_t y = 85, uint16_t color = COLOR_TEXT)
    {
        _tft.setTextDatum(TL_DATUM);
        _tft.setTextColor(color, COLOR_CARD);
        _tft.setTextSize(2);
        _tft.drawString(message, x, y);
    }

    void drawLabelValue(const String &label, const String &value, int16_t y,
                        uint16_t labelColor = COLOR_LABEL, uint16_t valueColor = COLOR_VALUE)
    {
        _tft.setTextDatum(TL_DATUM);
        _tft.setTextColor(labelColor, COLOR_CARD);
        _tft.setTextSize(2);
        _tft.drawString(label, 25, y);

        _tft.setTextDatum(TR_DATUM);
        _tft.setTextColor(valueColor, COLOR_CARD);
        _tft.setTextSize(2);
        _tft.drawString(value, 445, y);
    }

    void drawFooter(const String &text, uint16_t color = COLOR_TEXT)
    {
        _tft.setTextDatum(MC_DATUM);
        _tft.setTextColor(color, TFT_DARKGREY);
        _tft.setTextSize(1);
        _tft.drawString(text, 240, 300);
    }
}
namespace
{
    uint16_t _cursorX = 0;
    uint16_t _cursorY = 0;

    uint16_t _textColor = TFT_WHITE;
    uint16_t _bgColor = TFT_BLACK;
    uint8_t _textSize = 2;

    uint16_t lineHeight()
    {
        return 8 * _textSize;
    }

    uint16_t charWidth()
    {
        return 6 * _textSize;
    }

    void applyTextStyle()
    {
        _tft.setTextColor(_textColor, _bgColor);
        _tft.setTextSize(_textSize);
        _tft.setCursor(_cursorX, _cursorY);
    }

    void newLine()
    {
        _cursorX = 0;
        _cursorY += lineHeight();

        if (_cursorY + lineHeight() > _tft.height())
        {
            _tft.fillScreen(_bgColor);
            _cursorY = 0;
        }

        _tft.setCursor(_cursorX, _cursorY);
    }

    void writeChar(char c)
    {
        if (c == '\r')
            return;

        if (c == '\n')
        {
            newLine();
            return;
        }

        if (_cursorX + charWidth() > _tft.width())
        {
            newLine();
        }

        _tft.setCursor(_cursorX, _cursorY);
        _tft.print(c);
        _cursorX += charWidth();
    }

    void writeText(const String &text)
    {
        applyTextStyle();

        for (size_t i = 0; i < text.length(); i++)
        {
            writeChar(text[i]);
        }
    }
}

namespace tft_ili9488
{
    void initialize()
    {
        _tft.init();
        _tft.setRotation(1);
        _tft.fillScreen(TFT_BLACK);

        _textColor = TFT_WHITE;
        _bgColor = TFT_BLACK;
        _textSize = 2;
        _cursorX = 0;
        _cursorY = 0;

        applyTextStyle();
    }

    void clear()
    {
        _tft.fillScreen(_bgColor);
        _cursorX = 0;
        _cursorY = 0;
        _tft.setCursor(_cursorX, _cursorY);
    }

    void setTextColor(uint16_t fg, uint16_t bg)
    {
        _textColor = fg;
        _bgColor = bg;
        _tft.setTextColor(_textColor, _bgColor);
    }

    void setTextSize(uint8_t size)
    {
        if (size < 1)
            size = 1;
        _textSize = size;
        _tft.setTextSize(_textSize);
    }

    void setCursor(uint16_t x, uint16_t y)
    {
        _cursorX = x;
        _cursorY = y;
        _tft.setCursor(_cursorX, _cursorY);
    }

    void print(const String &text)
    {
        writeText(text);
    }

    void print(const char *text)
    {
        if (text == nullptr)
            return;
        writeText(String(text));
    }

    void print(char c)
    {
        writeChar(c);
    }

    void print(int value)
    {
        writeText(String(value));
    }

    void print(float value, uint8_t decimals)
    {
        writeText(String(value));
    }

    void println()
    {
        newLine();
    }

    void println(const String &text)
    {
        writeText(text);
        newLine();
    }

    void println(const char *text)
    {
        if (text != nullptr)
        {
            writeText(String(text));
        }
        newLine();
    }

    void println(char c)
    {
        writeChar(c);
        newLine();
    }

    void println(int value)
    {
        writeText(String(value));
        newLine();
    }

    void println(float value, uint8_t decimals)
    {
        writeText(String(value));
        newLine();
    }

    void initialize_production()
    {
        _tft.init();
        _tft.setRotation(1);
        _tft.fillScreen(production::COLOR_BG);
        _tft.setTextFont(2);
    }

    void draw_idle_screen()
    {
        production::drawBaseLayout("IDLE", TFT_DARKGREY);
        production::drawMessage("System is idle.", 25, 95);
        production::drawMessage("Press start from dashboard website", 25, 130);
        production::drawMessage("or use the physical button.", 25, 165);
        production::drawFooter("Waiting for user action");
    }
    void clear_prod_screen()
    {
        _tft.fillScreen(production::COLOR_BG);
    }
    void show_filling_tank_screen(float water_level_cm, float water_level_percent)
    {
        production::drawBaseLayout("FILLING TANK", TFT_BLUE);
        production::drawMessage("Filling process in progress...", 25, 90);
        production::drawLabelValue("Water level", String(water_level_cm, 1) + " cm", 145);
        production::drawLabelValue("Tank fill", String(water_level_percent, 1) + " %", 180);
        production::drawFooter("Realtime tank level monitoring", production::COLOR_WARNING);        
    }
    void show_raw_sampling_screen(float ph, float ntu, float ppm, float water_level_cm, float temp_c)
    {
        production::drawBaseLayout("RAW SAMPLING", TFT_MAGENTA);
        production::drawMessage("Sampling sensors data for initial reading", 25, 85);
        production::drawLabelValue("pH", String(ph, 2), 125);   
        production::drawLabelValue("Turbidity", String(ntu, 1) + " NTU", 155);
        production::drawLabelValue("TDS", String(ppm, 1) + " ppm", 185);
        production::drawLabelValue("Water level", String(water_level_cm, 1) + " cm", 215);
        production::drawLabelValue("Temperature", String(temp_c, 1) + " C", 245);

        production::drawFooter("Initial sensor acquisition");
    }
    void show_measure_moringa_screen(float dose_gram)
    {
        production::drawBaseLayout("MEASURE MORINGA", TFT_BROWN);
        production::drawMessage("Measuring moringa dose based on", 25, 100);
        production::drawMessage("current sensors data...", 25, 135);

        _tft.setTextDatum(MC_DATUM);
        _tft.setTextColor(production::COLOR_VALUE, production::COLOR_CARD);
        _tft.setTextSize(3);
        _tft.drawString(String(dose_gram, 2) + " gram", 240, 220);

        production::drawFooter("Dose calculation completed");        
    }
    void show_dosing_screen(float dose_gram)
    {
        production::drawBaseLayout("DOSING", TFT_ORANGE);
        production::drawMessage("Dosing the water with moringa...", 25, 105);

        _tft.setTextDatum(MC_DATUM);
        _tft.setTextColor(production::COLOR_VALUE, production::COLOR_CARD);
        _tft.setTextSize(3);
        _tft.drawString(String(dose_gram, 2) + " gram", 240, 190);

        production::drawFooter("Actuator is dispensing moringa");
    }
    void show_mixing_screen(uint32_t remaining_seconds)
    {
        production::drawBaseLayout("MIXING", TFT_CYAN);
        production::drawMessage("Mixing the water with moringa", 25, 100);

        _tft.setTextDatum(MC_DATUM);
        _tft.setTextColor(production::COLOR_VALUE, production::COLOR_CARD);
        _tft.setTextSize(4);
        _tft.drawString(production::formatTimeHHMMSS(remaining_seconds), 240, 190);

        production::drawFooter("Countdown mixing timer");        
    }
    void show_settling_screen(uint32_t remaining_seconds)
    {
       production::drawBaseLayout("SETTLING", TFT_NAVY);
        production::drawMessage("Waiting for flocs to settle", 25, 100);

        _tft.setTextDatum(MC_DATUM);
        _tft.setTextColor(TFT_GREENYELLOW, production::COLOR_CARD);
        _tft.setTextSize(4);
        _tft.drawString(production::formatTimeHHMMSS(remaining_seconds), 240, 190);

        production::drawFooter("Sedimentation countdown");       
    }
    void show_completed_screen(float final_ph, float final_ntu, float final_ppm)
    {
        production::drawBaseLayout("COMPLETED", TFT_GREEN);
        production::drawMessage("Process completed successfully", 25, 85, production::COLOR_SUCCESS);

        production::drawLabelValue("Final pH", String(final_ph, 2), 140, production::COLOR_LABEL, production::COLOR_SUCCESS);
        production::drawLabelValue("Final NTU", String(final_ntu, 1), 175, production::COLOR_LABEL, production::COLOR_SUCCESS);
        production::drawLabelValue("Final TDS", String(final_ppm, 1) + " ppm", 210, production::COLOR_LABEL, production::COLOR_SUCCESS);

        production::drawFooter("Treatment cycle finished", production::COLOR_SUCCESS);        
    }
    void show_failed_screen()
    {
        production::drawBaseLayout("FAILED", TFT_RED);
        production::drawMessage("Process failed", 25, 95, production::COLOR_ERROR);
        production::drawMessage("Reason:", 25, 145, production::COLOR_LABEL);
        production::drawMessage("SYSTEM ERROR", 25, 180, production::COLOR_TEXT);
        production::drawFooter("Check sensor, actuator, or network", production::COLOR_ERROR);
    }
}