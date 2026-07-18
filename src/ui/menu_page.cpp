#include "menu_page.h"
#include "displays/_display_manager.h"
#include "page_manager.h"

void MenuPage::on_enter()
{
    _selected_index = 0;
}

void MenuPage::render()
{
    _display_manager::draw_menu("Main Menu", _items, _selected_index);
}

void MenuPage::on_encoder_move(long delta)
{
    int count = static_cast<int>(_items.size());
    _selected_index = ((_selected_index + static_cast<int>(delta)) % count + count) % count;
}

void MenuPage::on_button_pressed()
{
    switch (_selected_index)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        page_manager::navigate_to(page_manager::PageType::TESTING_ACTUATOR);
        break;
    default:
        break;
    }
}
