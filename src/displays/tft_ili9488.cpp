#include "displays/tft_ili9488.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

static TFT_eSPI _tft = TFT_eSPI();

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
}