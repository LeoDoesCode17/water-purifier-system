#pragma once

const float kXrange = 2.f * 3.14159265359f;
const int FEATURES_NUM = 7;
constexpr float WATER_CLEAN_THRESHOLD = 0.5f;
extern const int kInferencesPerCycle;
extern const float scaler_mean[FEATURES_NUM];
extern const float scaler_scale[FEATURES_NUM];
extern const float DUMMY_DATA[FEATURES_NUM];
