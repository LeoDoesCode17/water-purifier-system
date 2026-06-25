#include "displays/tft_ili9488.h"
#include <TFT_eSPI.h>
#include <Arduino.h>
#include "config/constants.h"

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
    constexpr uint16_t SCREEN_W = 480;
    constexpr uint16_t SCREEN_H = 320;
    constexpr uint8_t HEADER_H = 50;
    constexpr uint8_t FOOTER_H = 40;
    constexpr uint16_t BODY_Y = HEADER_H;
    constexpr uint16_t BODY_H = SCREEN_H - HEADER_H - FOOTER_H;
    constexpr uint16_t COL_BG = TFT_BLACK;
    constexpr uint16_t COL_HEADER_BG = 0x0B4F; // dark teal
    constexpr uint16_t COL_FOOTER_BG = 0x1082; // dark gray
    constexpr uint16_t COL_ACCENT = 0x04D9;    // teal accent
    constexpr uint16_t COL_TEXT_PRI = TFT_WHITE;
    constexpr uint16_t COL_TEXT_MUT = 0xC618;   // light gray
    constexpr uint16_t COL_TEXT_FAINT = 0x7BEF; // mid gray
    constexpr uint16_t COL_IDLE_ICON = 0x04D9;  // teal for idle badge
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
        _tft.setRotation(1); // landscape: 480 wide, 320 tall
        _tft.fillScreen(production::COL_BG);

        draw_header();
        draw_footer();
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
    void _draw_labels()
    {
        _tft.fillScreen(TFT_BLACK);
        _tft.setTextColor(TFT_WHITE, TFT_BLACK);
        _tft.setTextSize(2);

        _tft.setCursor(20, 20);
        _tft.println("Sensor Monitor");

        _tft.setCursor(20, 70);
        _tft.println("pH:");

        _tft.setCursor(20, 110);
        _tft.println("TDS:");

        _tft.setCursor(20, 150);
        _tft.println("Turbidity:");

        _tft.setCursor(20, 190);
        _tft.println("Temp:");

        _tft.setCursor(20, 230);
        _tft.println("Distance:");
    }
    void _update_values(WaterQuality wq)
    {
        _tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        _tft.setTextSize(2);

        _tft.fillRect(180, 70, 250, 30, TFT_BLACK);
        _tft.setCursor(180, 70);
        _tft.print(wq.ph, 2);

        _tft.fillRect(180, 110, 250, 30, TFT_BLACK);
        _tft.setCursor(180, 110);
        _tft.print(wq.ppm, 1);
        _tft.print(" ppm");

        _tft.fillRect(180, 150, 250, 30, TFT_BLACK);
        _tft.setCursor(180, 150);
        _tft.print(wq.ntu, 2);
        _tft.print(" V");

        _tft.fillRect(180, 190, 250, 30, TFT_BLACK);
        _tft.setCursor(180, 190);
        _tft.print(wq.temperature, 2);
        _tft.print(" C");

        _tft.fillRect(180, 230, 250, 30, TFT_BLACK);
        _tft.setCursor(180, 230);
        _tft.print(wq.level, 1);
        _tft.print(" cm");
    }

    void _draw_centered_text(const char *text, int y, uint16_t color, uint8_t font)
    {
        _tft.setTextColor(color, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(text, production::SCREEN_W / 2, y, font);
    }
    void _draw_badge(int x, int y, int w, int h, uint16_t bg, uint16_t fg,
                     const char *label, uint8_t font)
    {
    }

    void draw_header(const char *title)
    {
        // Background bar
        _tft.fillRect(0, 0, production::SCREEN_W, production::HEADER_H, production::COL_HEADER_BG);

        // Left accent stripe
        _tft.fillRect(0, 0, 4, production::HEADER_H, production::COL_ACCENT);

        // Project name — left aligned
        _tft.setTextColor(production::COL_TEXT_PRI, production::COL_HEADER_BG);
        _tft.setTextDatum(ML_DATUM); // Middle-Left
        _tft.drawString(title, 14, production::HEADER_H / 2, 2);

        // Firmware label — right aligned
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_HEADER_BG);
        _tft.setTextDatum(MR_DATUM); // Middle-Right
        _tft.drawString("v1.0", production::SCREEN_W - 8, production::HEADER_H / 2, 2);

        // Bottom divider line
        _tft.drawFastHLine(0, production::HEADER_H - 1, production::SCREEN_W, production::COL_ACCENT);
    }
    void draw_footer(const char *status)
    {
        int y = production::SCREEN_H - production::FOOTER_H;

        _tft.fillRect(0, y, production::SCREEN_W, production::FOOTER_H, production::COL_FOOTER_BG);

        // Top divider
        _tft.drawFastHLine(0, y, production::SCREEN_W, production::COL_ACCENT);

        // Status label on left
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_FOOTER_BG);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("STATUS:", 10, y + production::FOOTER_H / 2, 1);

        // Status value
        _tft.setTextColor(production::COL_TEXT_PRI, production::COL_FOOTER_BG);
        _tft.drawString(status, 68, y + production::FOOTER_H / 2, 1);

        // Right side: system label
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_FOOTER_BG);
        _tft.setTextDatum(MR_DATUM);
        _tft.drawString("ESP32-S3 | Moringa Filter", production::SCREEN_W - 10, y + production::FOOTER_H / 2, 1);
    }

    // ── Body: one function per state ──────────────────────────────────────
    void state_idle_ui()
    {
        clear_body();
        update_footer_status("IDLE — Waiting");

        // ── State badge ───────────────────────────────────────────────────────
        // Rounded rect badge centered at top of body
        int badge_w = 120, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 14;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, production::COL_ACCENT);
        _tft.setTextColor(TFT_WHITE, production::COL_ACCENT);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("IDLE", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Subtitle ──────────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("System waiting for command", production::SCREEN_W / 2, production::BODY_Y + 70, 2);

        // ── Tank status cards ─────────────────────────────────────────────────
        // Card dimensions
        int card_w = 200, card_h = 56;
        int card_y = production::BODY_Y + 95;
        int card1_x = 20;
        int card2_x = production::SCREEN_W - card_w - 20;

        // Card outline
        _tft.drawRoundRect(card1_x, card_y, card_w, card_h, 6, production::COL_TEXT_FAINT);
        _tft.drawRoundRect(card2_x, card_y, card_w, card_h, 6, production::COL_TEXT_FAINT);

        // Card labels
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(TL_DATUM);
        _tft.drawString("MIXING TANK", card1_x + 10, card_y + 8, 1);
        _tft.drawString("SETTLING TANK", card2_x + 10, card_y + 8, 1);

        // Status dot + text
        uint16_t standby_color = 0x07E0; // green for standby
        _tft.fillCircle(card1_x + 14, card_y + 38, 5, standby_color);
        _tft.fillCircle(card2_x + 14, card_y + 38, 5, standby_color);

        _tft.setTextColor(standby_color, production::COL_BG);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("STANDBY", card1_x + 26, card_y + 38, 1);
        _tft.drawString("STANDBY", card2_x + 26, card_y + 38, 1);

        // ── MQTT hint at bottom of body ───────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Send START via MQTT to begin", production::SCREEN_W / 2, production::BODY_Y + 185, 1);
    }

    void state_filling_mixing_tank_ui(float water_level_cm)
    {
        clear_body();
        update_footer_status("Filling Mixing Tank...");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 210, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 14;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0xFD20);
        _tft.setTextColor(TFT_WHITE, 0xFD20);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("FILLING MIXING TANK", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Main card ─────────────────────────────────────────────────────────
        int card_x = 20, card_y = production::BODY_Y + 58;
        int card_w = production::SCREEN_W - 40, card_h = 148;
        _tft.drawRoundRect(card_x, card_y, card_w, card_h, 8, production::COL_TEXT_FAINT);

        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(TL_DATUM);
        _tft.drawString("MIXING TANK", card_x + 12, card_y + 10, 1);

        uint16_t pump_color = 0x07E0;
        _tft.fillCircle(card_x + card_w - 70, card_y + 15, 5, pump_color);
        _tft.setTextColor(pump_color, production::COL_BG);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("Pump 1  ON", card_x + card_w - 60, card_y + 15, 1);

        _tft.drawFastHLine(card_x + 1, card_y + 28, card_w - 2, production::COL_FOOTER_BG);

        // ── Water level computation ───────────────────────────────────────────
        // Sensor mounted at top: distance DECREASES as water rises.
        // Usable range: MAX (empty) → MIN (full)
        float dist = water_level_cm < constant::MIN_MIXING_TANK_LEVEL
                         ? constant::MIN_MIXING_TANK_LEVEL
                         : water_level_cm;
        dist = dist > constant::MAX_MIXING_TANK_LEVEL
                   ? constant::MAX_MIXING_TANK_LEVEL
                   : dist;

        float usable_cm = constant::MAX_MIXING_TANK_LEVEL - constant::MIN_MIXING_TANK_LEVEL; // 23 cm
        float filled_cm = constant::MAX_MIXING_TANK_LEVEL - dist;                            // 0 → 23 cm
        float ratio = filled_cm / usable_cm;                                                 // 0.0 → 1.0

        // ── Progress bar ──────────────────────────────────────────────────────
        int bar_x = card_x + 40;
        int bar_y = card_y + 42;
        int bar_w = card_w - 80;
        int bar_h = 22;
        int fill_w = (int)(ratio * bar_w);

        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MR_DATUM);
        _tft.drawString("0%", bar_x - 4, bar_y + bar_h / 2, 1);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("100%", bar_x + bar_w + 4, bar_y + bar_h / 2, 1);

        _tft.fillRoundRect(bar_x, bar_y, bar_w, bar_h, 4, production::COL_FOOTER_BG);

        uint16_t bar_fill_color = (ratio > 0.75f) ? 0x07FF : 0x001F; // cyan or blue
        if (fill_w > 0)
            _tft.fillRoundRect(bar_x, bar_y, fill_w, bar_h, 4, bar_fill_color);

        _tft.drawRoundRect(bar_x, bar_y, bar_w, bar_h, 4, production::COL_TEXT_FAINT);

        // ── Numeric reading — show filled height ──────────────────────────────
        char level_str[24];
        snprintf(level_str, sizeof(level_str), "%.1f cm", filled_cm);

        _tft.setTextColor(TFT_WHITE, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(level_str, production::SCREEN_W / 2, card_y + 84, 4);

        // ── Sub-labels ────────────────────────────────────────────────────────
        char pct_str[32];
        snprintf(pct_str, sizeof(pct_str), "%.0f%% of %.0f cm",
                 ratio * 100.0f, usable_cm);

        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(pct_str, production::SCREEN_W / 2, card_y + 116, 1);

        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.drawString("Filling in progress...", production::SCREEN_W / 2, card_y + 132, 1);
    }

    void state_raw_sampling_ui(const WaterQuality &wq)
    {
        clear_body();
        update_footer_status("Raw Sampling...");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 185, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 10;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0x001F); // blue
        _tft.setTextColor(TFT_WHITE, 0x001F);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("RAW SAMPLING", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Sensor card grid (2 rows x 3 cols) ───────────────────────────────
        // Each card: 140 x 60 px with 10px gap, centered in 480px width
        // Total width = 3*140 + 2*10 = 440, margin = (480-440)/2 = 20
        const int COLS = 3;
        const int CARD_W = 140;
        const int CARD_H = 62;
        const int GAP_X = 10;
        const int GAP_Y = 8;
        const int GRID_X = 20;
        const int GRID_Y = production::BODY_Y + 52;

        // Sensor definitions: label, value string, unit, accent color
        char v0[16], v1[16], v2[16], v3[16], v4[16], v5[16];
        snprintf(v0, sizeof(v0), "%.1f", wq.ntu);
        snprintf(v1, sizeof(v1), "%.2f", wq.ph);
        snprintf(v2, sizeof(v2), "%.0f", wq.ppm);
        snprintf(v3, sizeof(v3), "%.1f", wq.level);
        snprintf(v4, sizeof(v4), "%.1f", wq.temperature);
        snprintf(v5, sizeof(v5), "%.2f", wq.volume_liter);

        struct SensorCard
        {
            const char *label;
            const char *value;
            const char *unit;
            uint16_t accent;
        } cards[6] = {
            {"TURBIDITY", v0, "NTU", 0xFD20}, // orange
            {"pH", v1, "", 0x07E0},           // green
            {"TDS", v2, "ppm", 0x04D9},       // teal
            {"LEVEL", v3, "cm", 0x001F},      // blue
            {"TEMPERATURE", v4, "\xF7"
                                "C",
             0xF800},                    // red  (\xF7 = degree symbol font 1)
            {"VOLUME", v5, "L", 0x7BEF}, // gray
        };

        for (int i = 0; i < 6; i++)
        {
            int col = i % COLS;
            int row = i / COLS;
            int cx = GRID_X + col * (CARD_W + GAP_X);
            int cy = GRID_Y + row * (CARD_H + GAP_Y);

            // Card background + border
            _tft.fillRoundRect(cx, cy, CARD_W, CARD_H, 6, 0x1082); // dark gray fill
            _tft.drawRoundRect(cx, cy, CARD_W, CARD_H, 6, cards[i].accent);

            // Left accent stripe
            _tft.fillRect(cx, cy + 6, 3, CARD_H - 12, cards[i].accent);

            // Sensor label — top of card
            _tft.setTextColor(production::COL_TEXT_FAINT, 0x1082);
            _tft.setTextDatum(TL_DATUM);
            _tft.drawString(cards[i].label, cx + 10, cy + 7, 1);

            // Value — large, centered vertically in card
            _tft.setTextColor(TFT_WHITE, 0x1082);
            _tft.setTextDatum(ML_DATUM);
            _tft.drawString(cards[i].value, cx + 10, cy + 38, 2);

            // Unit — right-bottom of card, muted
            _tft.setTextColor(cards[i].accent, 0x1082);
            _tft.setTextDatum(BR_DATUM);
            _tft.drawString(cards[i].unit, cx + CARD_W - 6, cy + CARD_H - 5, 1);
        }

        // ── Hint text at bottom of body ───────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Collecting 50 samples, please wait...", production::SCREEN_W / 2, production::BODY_Y + 210, 1);
    }

    void state_ai_measure_ui(const float &moringa_dose_gram, const float &mixing_time_seconds)
    {
        clear_body();
        update_footer_status("AI Prediction Done");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 195, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 12;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0x781F); // purple
        _tft.setTextColor(TFT_WHITE, 0x781F);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("AI PREDICTION", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Subtitle ──────────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Moringa coagulant dose calculated", production::SCREEN_W / 2, production::BODY_Y + 60, 1);

        // ── Result cards ──────────────────────────────────────────────────────
        // Two equal cards side by side with gap
        int card_w = 200, card_h = 110;
        int card_y = production::BODY_Y + 75;
        int card1_x = (production::SCREEN_W / 2) - card_w - 10; // left card
        int card2_x = (production::SCREEN_W / 2) + 10;          // right card

        // ── Card 1: Moringa Dose ──────────────────────────────────────────────
        uint16_t dose_accent = 0x07E0; // green
        _tft.fillRoundRect(card1_x, card_y, card_w, card_h, 8, 0x1082);
        _tft.drawRoundRect(card1_x, card_y, card_w, card_h, 8, dose_accent);

        // Top accent bar
        _tft.fillRoundRect(card1_x, card_y, card_w, 4, 8, dose_accent);
        _tft.fillRect(card1_x, card_y + 2, card_w, 2, dose_accent);

        // Card label
        _tft.setTextColor(production::COL_TEXT_FAINT, 0x1082);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("MORINGA DOSE", card1_x + card_w / 2, card_y + 20, 1);

        // Value — large center
        char dose_str[16];
        snprintf(dose_str, sizeof(dose_str), "%.1f", moringa_dose_gram);
        _tft.setTextColor(TFT_WHITE, 0x1082);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(dose_str, card1_x + card_w / 2, card_y + 62, 4); // font 4 = large

        // Unit
        _tft.setTextColor(dose_accent, 0x1082);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("gram", card1_x + card_w / 2, card_y + 92, 2);

        // ── Card 2: Mixing Time ───────────────────────────────────────────────
        uint16_t time_accent = 0xFD20; // orange
        _tft.fillRoundRect(card2_x, card_y, card_w, card_h, 8, 0x1082);
        _tft.drawRoundRect(card2_x, card_y, card_w, card_h, 8, time_accent);

        // Top accent bar
        _tft.fillRoundRect(card2_x, card_y, card_w, 4, 8, time_accent);
        _tft.fillRect(card2_x, card_y + 2, card_w, 2, time_accent);

        // Card label
        _tft.setTextColor(production::COL_TEXT_FAINT, 0x1082);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("MIXING TIME", card2_x + card_w / 2, card_y + 20, 1);

        // Value — large center
        char time_str[16];
        snprintf(time_str, sizeof(time_str), "%.0f", mixing_time_seconds);
        _tft.setTextColor(TFT_WHITE, 0x1082);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(time_str, card2_x + card_w / 2, card_y + 62, 4);

        // Unit
        _tft.setTextColor(time_accent, 0x1082);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("seconds", card2_x + card_w / 2, card_y + 92, 2);

        // ── Bottom hint ───────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Proceeding to dosing stage...", production::SCREEN_W / 2, production::BODY_Y + 205, 1);
    }

    void state_dosing_ui(const float &dosing_time_elapsed_s,
                         const float &dosing_time_total_s)
    {
        clear_body();
        update_footer_status("Dosing...");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 140, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 12;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0xFFE0); // yellow
        _tft.setTextColor(TFT_BLACK, 0xFFE0);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("DOSING", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Subtitle ──────────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Servo valve open — dispensing moringa", production::SCREEN_W / 2, production::BODY_Y + 58, 1);

        // ── Servo status card ─────────────────────────────────────────────────
        int card_x = 60, card_y = production::BODY_Y + 72;
        int card_w = production::SCREEN_W - 120, card_h = 38;
        uint16_t valve_accent = 0x07E0; // green = open

        _tft.fillRoundRect(card_x, card_y, card_w, card_h, 6, 0x1082);
        _tft.drawRoundRect(card_x, card_y, card_w, card_h, 6, valve_accent);
        _tft.fillRect(card_x, card_y + 5, 3, card_h - 10, valve_accent);

        _tft.setTextColor(production::COL_TEXT_MUT, 0x1082);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("SERVO VALVE", card_x + 12, card_y + card_h / 2, 1);

        _tft.fillCircle(card_x + card_w - 85, card_y + card_h / 2, 5, valve_accent);
        _tft.setTextColor(valve_accent, 0x1082);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("OPEN  100%", card_x + card_w - 74, card_y + card_h / 2, 1);

        // ── Elapsed time (large number) ───────────────────────────────────────
        // Clamp to total
        float elapsed = dosing_time_elapsed_s < 0 ? 0 : dosing_time_elapsed_s;
        float total = dosing_time_total_s < 1 ? 1 : dosing_time_total_s;
        if (elapsed > total)
            elapsed = total;

        float ratio = elapsed / total;

        // Label above number
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("ELAPSED", production::SCREEN_W / 2, production::BODY_Y + 122, 1);

        // Large elapsed value — clear the area first to avoid digit ghosting
        _tft.fillRect((production::SCREEN_W / 2) - 80, production::BODY_Y + 128, 160, 42, production::COL_BG);
        char elapsed_str[12];
        snprintf(elapsed_str, sizeof(elapsed_str), "%.1f  s", elapsed);
        _tft.setTextColor(TFT_WHITE, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(elapsed_str, production::SCREEN_W / 2, production::BODY_Y + 148, 4);

        // ── Progress bar ──────────────────────────────────────────────────────
        int bar_x = 40, bar_y = production::BODY_Y + 178;
        int bar_w = production::SCREEN_W - 80, bar_h = 18;
        int fill_w = (int)(ratio * bar_w);

        _tft.fillRoundRect(bar_x, bar_y, bar_w, bar_h, 5, production::COL_FOOTER_BG);

        if (fill_w > 0)
        {
            // Color shifts yellow → green as it fills up
            uint16_t fill_color = (ratio > 0.8f) ? 0x07E0 : 0xFFE0;
            _tft.fillRoundRect(bar_x, bar_y, fill_w, bar_h, 5, fill_color);
        }

        _tft.drawRoundRect(bar_x, bar_y, bar_w, bar_h, 5, production::COL_TEXT_FAINT);

        // ── Progress label below bar ──────────────────────────────────────────
        // Clear label area to prevent text ghosting on every second update
        _tft.fillRect(bar_x, production::BODY_Y + 200, bar_w, 16, production::COL_BG);
        char prog_str[32];
        snprintf(prog_str, sizeof(prog_str), "%.1f / %.1f s", elapsed, total);
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(prog_str, production::SCREEN_W / 2, production::BODY_Y + 208, 1);
    }

    void state_mixing_ui(const float &mixing_time_elapsed_s,
                         const float &mixing_time_total_s)
    {
        clear_body();
        update_footer_status("Mixing...");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 140, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 12;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0x04D9); // teal
        _tft.setTextColor(TFT_WHITE, 0x04D9);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("MIXING", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Subtitle ──────────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Gear motor active — mixing coagulant", production::SCREEN_W / 2, production::BODY_Y + 58, 1);

        // ── Gear motor status card ────────────────────────────────────────────
        int card_x = 60, card_y = production::BODY_Y + 72;
        int card_w = production::SCREEN_W - 120, card_h = 38;
        uint16_t motor_accent = 0x07E0; // green = running

        _tft.fillRoundRect(card_x, card_y, card_w, card_h, 6, 0x1082);
        _tft.drawRoundRect(card_x, card_y, card_w, card_h, 6, motor_accent);
        _tft.fillRect(card_x, card_y + 5, 3, card_h - 10, motor_accent);

        _tft.setTextColor(production::COL_TEXT_MUT, 0x1082);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("GEAR MOTOR", card_x + 12, card_y + card_h / 2, 1);

        _tft.fillCircle(card_x + card_w - 75, card_y + card_h / 2, 5, motor_accent);
        _tft.setTextColor(motor_accent, 0x1082);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("RUNNING", card_x + card_w - 64, card_y + card_h / 2, 1);

        // ── Clamp values ──────────────────────────────────────────────────────
        float elapsed = mixing_time_elapsed_s < 0 ? 0 : mixing_time_elapsed_s;
        float total = mixing_time_total_s < 1 ? 1 : mixing_time_total_s;
        if (elapsed > total)
            elapsed = total;
        float ratio = elapsed / total;

        // ── Remaining time (large) ────────────────────────────────────────────
        float remaining_s = total - elapsed;

        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("REMAINING", production::SCREEN_W / 2, production::BODY_Y + 122, 1);

        // Convert remaining to MM:SS
        int rem_min = (int)(remaining_s / 60);
        int rem_sec = (int)(remaining_s) % 60;
        char remain_str[12];
        snprintf(remain_str, sizeof(remain_str), "%02d:%02d", rem_min, rem_sec);

        // Clear digit area to prevent ghosting
        _tft.fillRect((production::SCREEN_W / 2) - 70, production::BODY_Y + 128, 140, 42, production::COL_BG);
        _tft.setTextColor(TFT_WHITE, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(remain_str, production::SCREEN_W / 2, production::BODY_Y + 148, 4);

        // ── Progress bar ──────────────────────────────────────────────────────
        int bar_x = 40, bar_y = production::BODY_Y + 178;
        int bar_w = production::SCREEN_W - 80, bar_h = 18;
        int fill_w = (int)(ratio * bar_w);

        _tft.fillRoundRect(bar_x, bar_y, bar_w, bar_h, 5, production::COL_FOOTER_BG);

        if (fill_w > 0)
        {
            // Shifts teal → green as it nears completion
            uint16_t fill_color = (ratio > 0.85f) ? 0x07E0 : 0x04D9;
            _tft.fillRoundRect(bar_x, bar_y, fill_w, bar_h, 5, fill_color);
        }

        _tft.drawRoundRect(bar_x, bar_y, bar_w, bar_h, 5, production::COL_TEXT_FAINT);

        // ── Progress label below bar ──────────────────────────────────────────
        _tft.fillRect(bar_x, production::BODY_Y + 200, bar_w, 16, production::COL_BG);

        // Convert elapsed and total to MM:SS for label
        int el_min = (int)(elapsed / 60);
        int el_sec = (int)(elapsed) % 60;
        int to_min = (int)(total / 60);
        int to_sec = (int)(total) % 60;
        char prog_str[32];
        snprintf(prog_str, sizeof(prog_str), "%02d:%02d / %02d:%02d",
                 el_min, el_sec, to_min, to_sec);

        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(prog_str, production::SCREEN_W / 2, production::BODY_Y + 208, 1);
    }

    void state_filling_settling_tank_ui(float water_level_cm)
    {
        clear_body();
        update_footer_status("Filling Settling Tank...");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 230, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 14;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0x04D9);
        _tft.setTextColor(TFT_WHITE, 0x04D9);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("FILLING SETTLING TANK", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Main card ─────────────────────────────────────────────────────────
        int card_x = 20, card_y = production::BODY_Y + 58;
        int card_w = production::SCREEN_W - 40, card_h = 148;
        _tft.drawRoundRect(card_x, card_y, card_w, card_h, 8, production::COL_TEXT_FAINT);

        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(TL_DATUM);
        _tft.drawString("SETTLING TANK", card_x + 12, card_y + 10, 1);

        uint16_t pump_color = 0x07E0;
        _tft.fillCircle(card_x + card_w - 70, card_y + 15, 5, pump_color);
        _tft.setTextColor(pump_color, production::COL_BG);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("Pump 2  ON", card_x + card_w - 60, card_y + 15, 1);

        _tft.drawFastHLine(card_x + 1, card_y + 28, card_w - 2, production::COL_FOOTER_BG);

        // ── Water level computation ───────────────────────────────────────────
        float dist = water_level_cm < constant::MIN_SETTLING_TANK_LEVEL
                         ? constant::MIN_SETTLING_TANK_LEVEL
                         : water_level_cm;
        dist = dist > constant::MAX_SETTLING_TANK_LEVEL
                   ? constant::MAX_SETTLING_TANK_LEVEL
                   : dist;

        float usable_cm = constant::MAX_SETTLING_TANK_LEVEL - constant::MIN_SETTLING_TANK_LEVEL; // 21 cm
        float filled_cm = constant::MAX_SETTLING_TANK_LEVEL - dist;                              // 0 → 21 cm
        float ratio = filled_cm / usable_cm;                                                     // 0.0 → 1.0

        // ── Progress bar ──────────────────────────────────────────────────────
        int bar_x = card_x + 40;
        int bar_y = card_y + 42;
        int bar_w = card_w - 80;
        int bar_h = 22;
        int fill_w = (int)(ratio * bar_w);

        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MR_DATUM);
        _tft.drawString("0%", bar_x - 4, bar_y + bar_h / 2, 1);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("100%", bar_x + bar_w + 4, bar_y + bar_h / 2, 1);

        _tft.fillRoundRect(bar_x, bar_y, bar_w, bar_h, 4, production::COL_FOOTER_BG);

        uint16_t bar_fill_color = (ratio > 0.75f) ? 0x07FF : 0x04D9; // cyan or teal
        if (fill_w > 0)
            _tft.fillRoundRect(bar_x, bar_y, fill_w, bar_h, 4, bar_fill_color);

        _tft.drawRoundRect(bar_x, bar_y, bar_w, bar_h, 4, production::COL_TEXT_FAINT);

        // ── Numeric reading ───────────────────────────────────────────────────
        char level_str[24];
        snprintf(level_str, sizeof(level_str), "%.1f cm", filled_cm);

        _tft.setTextColor(TFT_WHITE, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(level_str, production::SCREEN_W / 2, card_y + 84, 4);

        // ── Sub-labels ────────────────────────────────────────────────────────
        char pct_str[32];
        snprintf(pct_str, sizeof(pct_str), "%.0f%% of %.0f cm",
                 ratio * 100.0f, usable_cm);

        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(pct_str, production::SCREEN_W / 2, card_y + 116, 1);

        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.drawString("Filling in progress...", production::SCREEN_W / 2, card_y + 132, 1);
    }
    
    void state_settling_ui(const WaterQuality &wq,
                           float elapsed_s,
                           float total_s)
    {
        clear_body();
        update_footer_status("Settling...");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 150, badge_h = 28;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 8;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0x4A49); // blue-gray
        _tft.setTextColor(TFT_WHITE, 0x4A49);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("SETTLING", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Timer card ────────────────────────────────────────────────────────
        float elapsed = elapsed_s < 0 ? 0 : elapsed_s;
        float total = total_s < 1 ? 1 : total_s;
        if (elapsed > total)
            elapsed = total;
        float ratio = elapsed / total;

        float remaining_s = total - elapsed;
        int rem_min = (int)(remaining_s / 60);
        int rem_sec = (int)(remaining_s) % 60;
        int el_min = (int)(elapsed / 60);
        int el_sec = (int)(elapsed) % 60;
        int to_min = (int)(total / 60);
        int to_sec = (int)(total) % 60;

        int tc_x = 20, tc_y = production::BODY_Y + 44;
        int tc_w = production::SCREEN_W - 40, tc_h = 48;
        uint16_t timer_accent = 0x4A49;

        _tft.fillRoundRect(tc_x, tc_y, tc_w, tc_h, 6, 0x1082);
        _tft.drawRoundRect(tc_x, tc_y, tc_w, tc_h, 6, timer_accent);
        _tft.fillRect(tc_x, tc_y + 5, 3, tc_h - 10, timer_accent);

        // "REMAINING" label
        _tft.setTextColor(production::COL_TEXT_FAINT, 0x1082);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString("REMAINING", tc_x + 12, tc_y + tc_h / 2, 1);

        // MM:SS countdown — clear area first
        _tft.fillRect(tc_x + 92, tc_y + 4, 100, tc_h - 8, 0x1082);
        char remain_str[10];
        snprintf(remain_str, sizeof(remain_str), "%02d:%02d", rem_min, rem_sec);
        _tft.setTextColor(TFT_WHITE, 0x1082);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString(remain_str, tc_x + 96, tc_y + tc_h / 2, 4);

        // Mini progress bar (right portion of timer card)
        int mb_x = tc_x + 210, mb_y = tc_y + 10;
        int mb_w = tc_w - 225, mb_h = 10;
        int mb_fill = (int)(ratio * mb_w);
        _tft.fillRoundRect(mb_x, mb_y, mb_w, mb_h, 3, production::COL_FOOTER_BG);
        if (mb_fill > 0)
        {
            uint16_t mb_color = (ratio > 0.85f) ? 0x07E0 : 0x4A49;
            _tft.fillRoundRect(mb_x, mb_y, mb_fill, mb_h, 3, mb_color);
        }
        _tft.drawRoundRect(mb_x, mb_y, mb_w, mb_h, 3, production::COL_TEXT_FAINT);

        // Elapsed / total label under mini bar
        char prog_str[20];
        snprintf(prog_str, sizeof(prog_str), "%02d:%02d / %02d:%02d",
                 el_min, el_sec, to_min, to_sec);
        _tft.setTextColor(production::COL_TEXT_FAINT, 0x1082);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString(prog_str, mb_x, mb_y + 18, 1);

        // ── Sensor card grid (2 rows x 3 cols) ───────────────────────────────
        // Smaller cards to fit under the timer card
        const int COLS = 3;
        const int CARD_W = 140, CARD_H = 52;
        const int GAP_X = 10, GAP_Y = 5;
        const int GRID_X = 20, GRID_Y = production::BODY_Y + 100;

        char v0[16], v1[16], v2[16], v3[16], v4[16], v5[16];
        snprintf(v0, sizeof(v0), "%.1f", wq.ntu);
        snprintf(v1, sizeof(v1), "%.2f", wq.ph);
        snprintf(v2, sizeof(v2), "%.0f", wq.ppm);
        snprintf(v3, sizeof(v3), "%.1f", wq.level);
        snprintf(v4, sizeof(v4), "%.1f", wq.temperature);
        snprintf(v5, sizeof(v5), "%.2f", wq.volume_liter);

        struct SensorCard
        {
            const char *label;
            const char *value;
            const char *unit;
            uint16_t accent;
        } cards[6] = {
            {"TURBIDITY", v0, "NTU", 0xFD20},
            {"pH", v1, "", 0x07E0},
            {"TDS", v2, "ppm", 0x04D9},
            {"LEVEL", v3, "cm", 0x001F},
            {"TEMPERATURE", v4, "\xF7"
                                "C",
             0xF800},
            {"VOLUME", v5, "L", 0x7BEF},
        };

        for (int i = 0; i < 6; i++)
        {
            int col = i % COLS;
            int row = i / COLS;
            int cx = GRID_X + col * (CARD_W + GAP_X);
            int cy = GRID_Y + row * (CARD_H + GAP_Y);

            _tft.fillRoundRect(cx, cy, CARD_W, CARD_H, 6, 0x1082);
            _tft.drawRoundRect(cx, cy, CARD_W, CARD_H, 6, cards[i].accent);
            _tft.fillRect(cx, cy + 5, 3, CARD_H - 10, cards[i].accent);

            _tft.setTextColor(production::COL_TEXT_FAINT, 0x1082);
            _tft.setTextDatum(TL_DATUM);
            _tft.drawString(cards[i].label, cx + 10, cy + 6, 1);

            _tft.setTextColor(TFT_WHITE, 0x1082);
            _tft.setTextDatum(ML_DATUM);
            _tft.drawString(cards[i].value, cx + 10, cy + 34, 2);

            _tft.setTextColor(cards[i].accent, 0x1082);
            _tft.setTextDatum(BR_DATUM);
            _tft.drawString(cards[i].unit, cx + CARD_W - 6, cy + CARD_H - 4, 1);
        }
    }

    void state_completed_ui()
    {
        clear_body();
        update_footer_status("Process Completed");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 185, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 18;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0x07E0); // green
        _tft.setTextColor(TFT_BLACK, 0x07E0);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("COMPLETED", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── Large checkmark icon (drawn with primitives) ───────────────────────
        // Center point of the check
        int cx = production::SCREEN_W / 2;
        int cy = production::BODY_Y + 100;
        int r = 30;

        // Outer circle
        _tft.drawCircle(cx, cy, r, 0x07E0);
        _tft.drawCircle(cx, cy, r - 1, 0x07E0); // double for thickness

        // Checkmark: short left leg + long right leg
        // Short leg: (cx-16, cy) → (cx-4, cy+12)
        // Long leg:  (cx-4,  cy+12) → (cx+16, cy-12)
        for (int t = 0; t <= 1; t++) // draw twice for 2px thickness
        {
            _tft.drawLine(cx - 16, cy + t, cx - 4, cy + 12 + t, 0x07E0);
            _tft.drawLine(cx - 4, cy + 12 + t, cx + 16, cy - 12 + t, 0x07E0);
        }

        // ── Main message ──────────────────────────────────────────────────────
        _tft.setTextColor(TFT_WHITE, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Purification Complete!", production::SCREEN_W / 2, production::BODY_Y + 148, 2);

        // ── Sub-messages ──────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("All actuators turned off", production::SCREEN_W / 2, production::BODY_Y + 172, 1);
        _tft.drawString("Result published via MQTT", production::SCREEN_W / 2, production::BODY_Y + 188, 1);

        // ── "Returning to IDLE" note ──────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Returning to IDLE...", production::SCREEN_W / 2, production::BODY_Y + 212, 1);
    }

    void state_failed_ui(const char *reason)
    {
        clear_body();
        update_footer_status("Process Failed");

        // ── State badge ───────────────────────────────────────────────────────
        int badge_w = 150, badge_h = 32;
        int badge_x = (production::SCREEN_W - badge_w) / 2;
        int badge_y = production::BODY_Y + 18;
        _tft.fillRoundRect(badge_x, badge_y, badge_w, badge_h, 8, 0xF800); // red
        _tft.setTextColor(TFT_WHITE, 0xF800);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("FAILED", production::SCREEN_W / 2, badge_y + badge_h / 2, 2);

        // ── X icon (drawn with primitives) ────────────────────────────────────
        int cx = production::SCREEN_W / 2;
        int cy = production::BODY_Y + 100;
        int r = 30;
        int o = 16; // offset for X arms

        // Outer circle
        _tft.drawCircle(cx, cy, r, 0xF800);
        _tft.drawCircle(cx, cy, r - 1, 0xF800);

        // X mark: two diagonal lines, drawn twice for 2px thickness
        for (int t = 0; t <= 1; t++)
        {
            _tft.drawLine(cx - o, cy - o + t, cx + o, cy + o + t, 0xF800); // top-left → bottom-right
            _tft.drawLine(cx + o, cy - o + t, cx - o, cy + o + t, 0xF800); // top-right → bottom-left
        }

        // ── Main message ──────────────────────────────────────────────────────
        _tft.setTextColor(TFT_WHITE, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Process Failed!", production::SCREEN_W / 2, production::BODY_Y + 148, 2);

        // ── Reason string ─────────────────────────────────────────────────────
        _tft.setTextColor(0xFD20, production::COL_BG); // orange — stands out without being more alarming
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString(reason, production::SCREEN_W / 2, production::BODY_Y + 170, 1);

        // ── Sub-messages ──────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_MUT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("All actuators turned off", production::SCREEN_W / 2, production::BODY_Y + 190, 1);
        _tft.drawString("Error reported via MQTT", production::SCREEN_W / 2, production::BODY_Y + 206, 1);

        // ── Return note ───────────────────────────────────────────────────────
        _tft.setTextColor(production::COL_TEXT_FAINT, production::COL_BG);
        _tft.setTextDatum(MC_DATUM);
        _tft.drawString("Returning to IDLE...", production::SCREEN_W / 2, production::BODY_Y + 218, 1);
    }

    // ── Helpers ───────────────────────────────────────────────────────────
    void clear_body()
    {
        _tft.fillRect(0, production::BODY_Y, production::SCREEN_W, production::BODY_H, production::COL_BG);
    }
    void update_footer_status(const char *status)
    {
        int y = production::SCREEN_H - production::FOOTER_H;

        // Clear only the status value area
        _tft.fillRect(68, y + 4, 160, production::FOOTER_H - 8, production::COL_FOOTER_BG);

        _tft.setTextColor(production::COL_TEXT_PRI, production::COL_FOOTER_BG);
        _tft.setTextDatum(ML_DATUM);
        _tft.drawString(status, 68, y + production::FOOTER_H / 2, 1);
    }
    void update_header_time(const char *time_str)
    {

    } // optional clock
}