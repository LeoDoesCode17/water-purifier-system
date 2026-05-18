#include "displays/tft_terminal.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

static TFT_eSPI _tft;

// ── Terminal scroll state ─────────────────────────────────────────────────────
static int _char_height = 16;
static int _max_lines   = 20;
static int _cur_line    = 0;
static int _line_count  = 0;
static int _cur_x       = 2;

static void _newline() {
    _cur_x = 2;
    _line_count++;
    if ((_line_count % _max_lines) == 0) {
        _tft.fillScreen(TFT_BLACK);
    }
    _cur_line = _line_count % _max_lines;
}

// ── Table layout constants ────────────────────────────────────────────────────
// Display is 480x320 (landscape). Divide into 3 columns:
//   Col 0 (label):   x=0,   width=140
//   Col 1 (sensor1): x=140, width=170
//   Col 2 (sensor2): x=310, width=170
#define TABLE_X0        0
#define TABLE_X1        140
#define TABLE_X2        310
#define TABLE_ROW_H     42      // height per row (px)
#define TABLE_HEADER_Y  0       // Y start of header row
#define TABLE_ROW_START 42      // Y start of first data row
#define TABLE_PAD_X     6       // inner horizontal padding
#define TABLE_PAD_Y     10      // inner vertical padding (centers text in row)

#define COL_BG          TFT_BLACK
#define HEADER_BG       0x2945  // dark teal
#define ROW_BG_ODD      0x1082  // dark grey
#define ROW_BG_EVEN     TFT_BLACK
#define LABEL_FG        TFT_WHITE
#define VALUE_FG        TFT_GREEN
#define HEADER_FG       TFT_CYAN
#define DIVIDER_COLOR   0x4208  // mid grey

static const char* ROW_LABELS[] = {
    "Temperature (C)",
    "Water Level (cm)",
    "NTU (Volts)",
    "PPM",
    "PH"
};
static const int ROW_COUNT = 5;

// Draw a filled cell with optional top/left border lines
static void _draw_cell(int x, int w, int y, int h, uint16_t bg) {
    _tft.fillRect(x, y, w, h, bg);
    // right divider line
    _tft.drawFastVLine(x + w - 1, y, h, DIVIDER_COLOR);
    // bottom divider line
    _tft.drawFastHLine(x, y + h - 1, w, DIVIDER_COLOR);
}

// Draw the static scaffold: header row + label column (only call once)
static void _draw_table_scaffold() {
    _tft.fillScreen(TFT_BLACK);

    // ── Header row ────────────────────────────────────────────────────────────
    _draw_cell(TABLE_X0, TABLE_X1 - TABLE_X0, TABLE_HEADER_Y, TABLE_ROW_H, HEADER_BG);
    _draw_cell(TABLE_X1, TABLE_X2 - TABLE_X1, TABLE_HEADER_Y, TABLE_ROW_H, HEADER_BG);
    _draw_cell(TABLE_X2, 480 - TABLE_X2,      TABLE_HEADER_Y, TABLE_ROW_H, HEADER_BG);

    _tft.setTextFont(2);
    _tft.setTextSize(1);
    _tft.setTextColor(HEADER_FG, HEADER_BG);
    _tft.drawString("Data",     TABLE_X0 + TABLE_PAD_X, TABLE_HEADER_Y + TABLE_PAD_Y);
    _tft.drawString("Sensor 1", TABLE_X1 + TABLE_PAD_X, TABLE_HEADER_Y + TABLE_PAD_Y);
    _tft.drawString("Sensor 2", TABLE_X2 + TABLE_PAD_X, TABLE_HEADER_Y + TABLE_PAD_Y);

    // ── Label column + empty value cells ─────────────────────────────────────
    for (int i = 0; i < ROW_COUNT; i++) {
        int y    = TABLE_ROW_START + i * TABLE_ROW_H;
        uint16_t row_bg = (i % 2 == 0) ? ROW_BG_EVEN : ROW_BG_ODD;

        _draw_cell(TABLE_X0, TABLE_X1 - TABLE_X0, y, TABLE_ROW_H, row_bg);
        _draw_cell(TABLE_X1, TABLE_X2 - TABLE_X1, y, TABLE_ROW_H, row_bg);
        _draw_cell(TABLE_X2, 480 - TABLE_X2,      y, TABLE_ROW_H, row_bg);

        _tft.setTextColor(LABEL_FG, row_bg);
        _tft.drawString(ROW_LABELS[i], TABLE_X0 + TABLE_PAD_X, y + TABLE_PAD_Y);
    }
}

// Overwrite only the value cell — clears old value before drawing new one
static void _update_value(int col_x, int col_w, int row_index, float value, int decimals = 2) {
    int y        = TABLE_ROW_START + row_index * TABLE_ROW_H;
    uint16_t bg  = (row_index % 2 == 0) ? ROW_BG_EVEN : ROW_BG_ODD;

    // Erase previous value (leave border lines intact by shrinking 1px)
    _tft.fillRect(col_x + 1, y + 1, col_w - 2, TABLE_ROW_H - 2, bg);

    _tft.setTextColor(VALUE_FG, bg);
    _tft.drawString(String(value, decimals), col_x + TABLE_PAD_X, y + TABLE_PAD_Y);
}

// ── Public API ────────────────────────────────────────────────────────────────
namespace tft_terminal {

    void begin() {
        _tft.init();
        _tft.setRotation(1);   // Landscape 480x320
        _tft.fillScreen(TFT_BLACK);
        _tft.setTextFont(2);
        _tft.setTextSize(1);
        _tft.setTextColor(TFT_GREEN, TFT_BLACK);
        _tft.setTextDatum(TL_DATUM);

        _char_height = _tft.fontHeight(2) + 2;
        _max_lines   = _tft.height() / _char_height;
        _cur_line    = 0;
        _line_count  = 0;
        _cur_x       = 2;
    }

    void print(const String& text) {
        _tft.setTextColor(TFT_GREEN, TFT_BLACK);
        _tft.drawString(text, _cur_x, _cur_line * _char_height);
        _cur_x += _tft.textWidth(text);
    }

    void println(const String& text) {
        if (text.length() > 0) print(text);
        _newline();
    }

    void draw_table(const WaterQuality& s1, const WaterQuality& s2) {
        static bool _scaffold_drawn = false;
        if (!_scaffold_drawn) {
            _draw_table_scaffold();
            _scaffold_drawn = true;
        }

        // Row order matches ROW_LABELS: Temperature, Water Level, NTU, PPM, PH
        int w1 = TABLE_X2 - TABLE_X1;   // col 1 width
        int w2 = 480 - TABLE_X2;        // col 2 width

        _update_value(TABLE_X1, w1, 0, s1.temperature, 2);
        _update_value(TABLE_X2, w2, 0, s2.temperature, 2);

        _update_value(TABLE_X1, w1, 1, s1.level, 2);
        _update_value(TABLE_X2, w2, 1, s2.level, 2);

        _update_value(TABLE_X1, w1, 2, s1.ntu, 2);
        _update_value(TABLE_X2, w2, 2, s2.ntu, 2);

        _update_value(TABLE_X1, w1, 3, s1.ppm, 2);
        _update_value(TABLE_X2, w2, 3, s2.ppm, 2);

        _update_value(TABLE_X1, w1, 4, s1.ph, 3);
        _update_value(TABLE_X2, w2, 4, s2.ph, 3);
    }
}