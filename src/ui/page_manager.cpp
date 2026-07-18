#include "page_manager.h"
#include "menu_page.h"
#include "sensors/sensor_manager.h"

namespace
{
    MenuPage menu_page;
    Page *current_page = nullptr;
    long last_encoder_position = 0;
} // namespace

namespace page_manager
{

    void initialize()
    {
        navigate_to(PageType::MENU);
    }

    void update()
    {
        long current_position = sensor_manager::get_encoder_position();
        long delta = current_position - last_encoder_position;

        if (delta != 0)
        {
            last_encoder_position = current_position;
            current_page->on_encoder_move(delta);
            current_page->render();
        }

        if (sensor_manager::was_encoder_button_pressed())
        {
            current_page->on_button_pressed();
            current_page->render();
        }
    }

    void navigate_to(PageType page_type)
    {
        switch (page_type)
        {
        case PageType::MENU:
            current_page = &menu_page;
            break;
        case PageType::TESTING_ACTUATOR:
            break;
        default:
            break;
        }

        sensor_manager::set_encoder_position(0);
        last_encoder_position = 0;
        current_page->on_enter();
        current_page->render();
    }
}
