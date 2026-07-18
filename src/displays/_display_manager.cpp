#include "_display_manager.h"

namespace
{
    TFT_eSPI tft = TFT_eSPI();

    constexpr int ROW_HEIGHT = 32;
    constexpr int LIST_START_Y = 40;
    constexpr int LEFT_MARGIN = 10;
}

namespace _display_manager
{
    void initialize()
    {
        tft.init();
        tft.setRotation(1); // landscape; adjust to 0-3 as needed
        tft.fillScreen(TFT_BLACK);
        tft.setTextFont(2);
        tft.setTextSize(1);
    }
    TFT_eSPI &get_tft()
    {
        return tft;
    }
    void draw_menu(
        const char *title,
        const std::vector<String> &labels,
        int selected_index,
        String (*status_of)(int) = nullptr)
    {
        tft.fillScreen(TFT_BLACK);

        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextDatum(TL_DATUM);
        tft.drawString(title, LEFT_MARGIN, 8, 4);
        tft.drawFastHLine(0, 32, tft.width(), TFT_DARKGREY);

        for (size_t i = 0; i < labels.size(); i++)
        {
            int y = LIST_START_Y + static_cast<int>(i) * ROW_HEIGHT;
            bool isSelected = (static_cast<int>(i) == selected_index);

            if (isSelected)
            {
                tft.fillRect(0, y, tft.width(), ROW_HEIGHT, TFT_WHITE);
                tft.setTextColor(TFT_BLACK, TFT_WHITE); // negative color cursor
            }
            else
            {
                tft.fillRect(0, y, tft.width(), ROW_HEIGHT, TFT_BLACK);
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }

            tft.drawString(labels[i], LEFT_MARGIN, y + 8, 2);

            if (status_of != nullptr)
            {
                String status = status_of(static_cast<int>(i));
                if (status.length() > 0)
                {
                    tft.setTextDatum(TR_DATUM);
                    tft.drawString(status, tft.width() - LEFT_MARGIN, y + 8, 2);
                    tft.setTextDatum(TL_DATUM);
                }
            }
        }
    }
}