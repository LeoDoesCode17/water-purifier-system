#pragma once
#include "page.h"

namespace page_manager {
    enum class PageType {
        MENU,
        TESTING_ACTUATOR
    };

    void initialize();
    void navigate_to(PageType page_type);
    void update(); // will call every loop  
}