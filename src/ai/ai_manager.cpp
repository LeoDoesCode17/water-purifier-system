#include "ai/ai_manager.h"
#include <Arduino.h>
static float moringa_dose_gram = 0.0;
static float mixing_time_seconds = 0.0;
namespace ai_manager
{
    void initialize()
    {
    }
    void predict(const WaterQuality &wq)
    {
        // TODO: implement the prediciton using WQ by modifying the value of moringa_dose_gram and mixing_time_seconds
        delay(2000);
    }
    float get_moringa_dose_gram()
    {
        return moringa_dose_gram;
    }
    float get_mixing_time_second()
    {
        return mixing_time_seconds;
    }
}