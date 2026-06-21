#ifndef AI_AI_MANAGER_H
#define AI_AI_MANAGER_H

#include "config/types.h"
namespace ai_manager
{
    void initialize();
    void predict(const WaterQuality &wq);
    float get_moringa_dose_gram();
    float get_mixing_time_second();
}
#endif