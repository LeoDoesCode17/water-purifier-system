#pragma once
#include <TFT_eSPI.h>
#include <vector>
#include <String.h>

namespace _display_manager
{
    void initialize();
    TFT_eSPI &get_tft();
    // Renders a vertical list menu with a highlighted cursor row.
    // labels: text of each row. selectedIndex: which row is highlighted.
    // status_of(i): optional callback returning suffix text (e.g. ON/OFF) for row i, pass nullptr if unused.
    void draw_menu(
        const char *title,
        const std::vector<String> &labels,
        int selected_index,
        String (*status_of)(int) = nullptr);
}