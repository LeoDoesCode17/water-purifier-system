#pragma once

#include <cstddef>
#include "types.h"
#include "config/types.h"

namespace ai_manager
{
    // Call once from setup(). Returns false if model loading / tensor
    // allocation failed — check this before calling run().
    bool initialize();

    // Runs one inference pass on the given feature vector.
    // features must have exactly FEATURES_NUM elements (see ai/constants.h).
    InferenceResult run(const float *features, size_t feature_count);

    // Optional: prints input/output tensor scale & zero_point to Serial.
    // Useful for sanity-checking against what you expect from training.
    void print_tensor_params();

    // old apis
    void predict(const WaterQuality &wq);
    float get_moringa_dose_gram();
    float get_mixing_time_second();
}