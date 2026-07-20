#include "ai_manager.h"

#include <Arduino.h>
#include <cmath>

#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model.h"
#include "constants.h"

namespace ai_manager
{
    namespace
    {
        tflite::ErrorReporter *error_reporter = nullptr;
        const tflite::Model *model = nullptr;
        tflite::MicroInterpreter *interpreter = nullptr;
        TfLiteTensor *input = nullptr;
        TfLiteTensor *output = nullptr;

        static float mixing_time_seconds = 0, moringa_dose_gram = 0;

        constexpr int kTensorArenaSize = 2000;
        alignas(16) uint8_t tensor_arena[kTensorArenaSize];

        bool initialized = false;
    }

    bool initialize()
    {
        static tflite::MicroErrorReporter micro_error_reporter;
        error_reporter = &micro_error_reporter;

        model = tflite::GetModel(g_model);
        if (model->version() != TFLITE_SCHEMA_VERSION)
        {
            TF_LITE_REPORT_ERROR(error_reporter,
                                 "Model provided is schema version %d not equal "
                                 "to supported version %d.",
                                 model->version(), TFLITE_SCHEMA_VERSION);
            return false;
        }

        // NOLINTNEXTLINE(runtime-global-variables)
        static tflite::AllOpsResolver resolver;

        static tflite::MicroInterpreter static_interpreter(
            model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
        interpreter = &static_interpreter;

        if (interpreter->AllocateTensors() != kTfLiteOk)
        {
            TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
            return false;
        }

        input = interpreter->input(0);
        output = interpreter->output(0);

        initialized = true;
        return true;
    }

    InferenceResult run(const float *features, size_t feature_count)
    {
        InferenceResult result;

        if (!initialized)
        {
            TF_LITE_REPORT_ERROR(error_reporter, "AiManager::run() called before init()");
            return result;
        }

        if (feature_count != FEATURES_NUM)
        {
            TF_LITE_REPORT_ERROR(error_reporter,
                                 "Feature count mismatch: got %d, expected %d",
                                 static_cast<int>(feature_count), FEATURES_NUM);
            return result;
        }

        for (size_t i = 0; i < feature_count; i++)
        {
            float x_scaled = (features[i] - scaler_mean[i]) / scaler_scale[i];
            input->data.int8[i] = static_cast<int8_t>(
                std::round(x_scaled / input->params.scale) + input->params.zero_point);
        }

        if (interpreter->Invoke() != kTfLiteOk)
        {
            TF_LITE_REPORT_ERROR(error_reporter, "Invoke() failed");
            return result;
        }

        int8_t y_quantized = output->data.int8[0];
        float y = (y_quantized - output->params.zero_point) * output->params.scale;

        result.success = true;
        result.value = y;
        result.is_water_clean = (y >= WATER_CLEAN_THRESHOLD);
        return result;
    }

    void print_tensor_params()
    {
        if (!initialized)
            return;

        Serial.println("======= INPUT PARAMS ========");
        Serial.printf("Scale\t\t: %.6f\n", input->params.scale);
        Serial.printf("Zero point\t: %d\n", input->params.zero_point);
        Serial.println("======== OUTPUT PARAMS =======");
        Serial.printf("Scale\t\t: %.6f\n", output->params.scale);
        Serial.printf("Zero point\t: %d\n", output->params.zero_point);
        Serial.println("==============================");
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