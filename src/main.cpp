#include <Arduino.h>
#include <ArduinoJson.h>
#include "config/pins.h"
#include "config/constants.h"
#include "config/secrets.h"
#include "sensors/sensor_manager.h"
#include "actuators/actuator_manager.h"
#include "networks/network_manager.h"
#include "config/types.h"
#include "displays/display_manager.h"
#include "ai/ai_manager.h"

typedef unsigned long ul;
ul lastReconnectAttempt = 0;

// TIMING
ul last_publish_settling_data = 0;
ul last_publish_status = 0;
ul last_main_loop = 0;
ul last_settling_time = 0;
ul last_dosing_moringa = 0;
ul last_mixing_moringa = 0;
ul opening_servo_valve_time_ms = 0;
ul current_time = 0;

bool is_development = false;
enum state_machine state_machine = IDLE;

WaterQuality global_wq = {
    .ntu = 0.0,
    .ph = 0.0,
    .ppm = 0.0,
    .level = 0.0,
    .temperature = 0.0};
WaterQuality last_settling_wq = {};

float global_moringa_dose_gram = 0;
float global_mixing_time_seconds = 0;

ul time_now = millis();

// ON MESSAGE FOR DEVELOPING
void handle_collect_data(JsonDocument &doc)
{
  float coagulant_dose = doc["coagulant_dose"] | -1.0f;
  int time_mixing = doc["time_mixing"] | -1;

  if (coagulant_dose < 0 || time_mixing < 0)
  {
    Serial.println("[EVENT] Missing or invalid fields in collect_data payload");
    return;
  }
  Serial.print("[EVENT] Coagulant dose : ");
  Serial.print(coagulant_dose);
  Serial.println(" g");

  Serial.print("[EVENT] Time mixing    : ");
  Serial.print(time_mixing);
  Serial.println(" min");

  global_mixing_time_seconds = time_mixing * 60.0;
  global_moringa_dose_gram = coagulant_dose;

  state_machine = DOSING;
}

void handle_control_actuator(JsonDocument &doc)
{
  int actuator_id = doc["actuator"] | -1;
  int value = doc["value"] | -1;

  if (actuator_id < 0 || actuator_id >= constant::ACTUATOR_COUNT)
  {
    Serial.print("[ACTUATOR] Unknown actuator id: ");
    Serial.println(actuator_id);
    return;
  }

  if (value != 0 && value != 1)
  {
    Serial.println("[ACTUATOR] Invalid value, must be 0 or 1");
    return;
  }

  const char *actuator = constant::ACTUATOR_NAMES[actuator_id];
  if (strcmp(actuator, "pump_1") == 0)
    value ? actuator_manager::turn_on_pump_1() : actuator_manager::turn_off_pump_1();
  else if (strcmp(actuator, "pump_2") == 0)
    value ? actuator_manager::turn_on_pump_2() : actuator_manager::turn_off_pump_2();
  else if (strcmp(actuator, "gear_motor") == 0)
    value ? actuator_manager::turnOnGearMotor() : actuator_manager::turnOffGearMotor();
  else if (strcmp(actuator, "servo_valve") == 0)
    value ? actuator_manager::openServoValve() : actuator_manager::closeServoValve();

  Serial.print("[ACTUATOR] ");
  Serial.print(actuator);
  Serial.print(" -> ");
  Serial.println(value ? "ON" : "OFF");
}

void handle_abort_system()
{
  state_machine = COMPLETED;
}

void on_mqtt_message(char *topic, uint8_t *payload, unsigned int length)
{
  Serial.println("\n========== MQTT MESSAGE RECEIVED ==========");
  Serial.print("[MQTT] Topic   : ");
  Serial.println(topic);

  Serial.print("[MQTT] Payload : ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, (const byte *)payload, length);

  if (error)
  {
    Serial.print("[MQTT] JSON parse failed: ");
    Serial.println(error.c_str());
    Serial.println("===========================================\n");
    return;
  }

  if (!doc.containsKey("event_type"))
  {
    Serial.println("[MQTT] Missing 'event_type' key");
    Serial.println("===========================================\n");
    return;
  }

  int event_type = doc["event_type"] | -1;

  switch (event_type)
  {
  case 0:
    Serial.println("[EVENT] Type: collect_data");
    handle_collect_data(doc);
    break;

  case 1:
    Serial.println("[EVENT] Type: control_actuator");
    handle_control_actuator(doc);
    break;

  case 999:
    Serial.println("[EVENT] Type: abort system");
    handle_abort_system();
    break;

  default:
    Serial.print("[EVENT] Unknown event_type: ");
    Serial.println(event_type);
    break;
  }

  Serial.println("===========================================\n");
}
// END ON MESSAGE FOR DEVELOPING

void setup()
{
  Serial.begin(constant::BAUD_RATE);

  is_development ? display_manager::initialize_development() : display_manager::initialize_production();

  sensor_manager::initialize_all();
  actuator_manager::initialize();
  delay(1000);

  display_manager::_draw_labels();

  // network_manager::initialize();
  // network_manager::mqtt_set_callback(on_mqtt_message);
  // network_manager::mqtt_connect();
  // delay(1000);

  // ai_manager::initialize();
}

void loop()
{
  ul current_millis = millis();
  if (current_millis - last_main_loop >= constant::MAIN_LOOP_INTERVAL_MS)
  {
    last_main_loop = millis();

    network_manager::mqtt_reconnect();
    network_manager::wifi_reconnect();

    // STATE MACHINE IDLE
    if (state_machine == IDLE)
    {
      display_manager::state_idle_ui();
      if (millis() - last_publish_status >= constant::PUBLISH_STATUS_INTERVAL_MS)
      {
        last_publish_status = millis();
        network_manager::mqtt_publish_status();
      }

      // display_manager::ili9488_println("[STATE MACHINE] State is in IDLE");
      // TODO: MUST BE ACTIVATED IF BUTTON IS PRESSED BUT FOR NOW AUTOMATICALLY START
      state_machine = FILLING_MIXING_TANK;
      // display_manager::ili9488_println("[STATE MACHINE] State is in FILLING_MIXING_TANK");
      // display_manager::ili9488_println("[ACTUATOR] Turn on pump 1");
      actuator_manager::turn_on_pump_1();
    }

    // STATE MACHINE FILLING MIXING TANK
    else if (state_machine == FILLING_MIXING_TANK)
    {
      float water_level = sensor_manager::get_distance_ultrasonic_1();
      display_manager::state_filling_mixing_tank_ui(water_level);
      if (water_level <= constant::MAX_MIXING_TANK_LEVEL)
      {
        actuator_manager::turn_off_pump_1();
        state_machine = RAW_SAMPLING;
      }
      // display_manager::ili9488_print("[FILLING TANK] Filling the tank until ");
      // display_manager::ili9488_println(constant::MAX_MIXING_TANK_LEVEL, 2);
      Serial.print("[FILLING TANK] Filling the tank until ");
      Serial.println(constant::MAX_MIXING_TANK_LEVEL, 2);
    }

    // STATE MACHINE RAW SAMPLING
    else if (state_machine == RAW_SAMPLING)
    {
      // display_manager::ili9488_println("[RAW SAMPLING] Sampling data sensor");
      // Accumulators
      float sum_ph = 0;
      float sum_ntu = 0;
      float sum_ppm = 0;
      float sum_level = 0;
      float sum_temperature = 0;

      for (int i = 0; i < constant::N_SAMPLES; i++)
      {
        sensor_manager::update_all();

        float ph = sensor_manager::get_ph_value_1();
        float ntu = sensor_manager::get_turbidity_ntu_value_1();
        float ppm = sensor_manager::get_tds_ppm_value_1();
        float level = sensor_manager::get_distance_ultrasonic_1();
        float temp = sensor_manager::get_temperature_c_1();

        sum_ph += ph;
        sum_ntu += ntu;
        sum_ppm += ppm;
        sum_level += level;
        sum_temperature += temp;

        // Build running average WaterQuality for live display
        float avg_level = sum_level / (i + 1);
        WaterQuality live_wq = {
            .ntu = sum_ntu / (i + 1),
            .ph = sum_ph / (i + 1),
            .ppm = sum_ppm / (i + 1),
            .level = avg_level,
            .temperature = sum_temperature / (i + 1),
            .volume_liter = sensor_manager::get_water_volume_liter(avg_level),
        };

        // Update UI every 5 samples to reduce flicker and SPI bus load
        if (i % 5 == 0 || i == constant::N_SAMPLES - 1)
        {
          display_manager::state_raw_sampling_ui(live_wq);
        }

        delay(constant::SAMPLE_DELAY_MS);
      }

      // Final averaged result
      float final_level = sum_level / constant::N_SAMPLES;
      global_wq = {
          .ntu = sum_ntu / constant::N_SAMPLES,
          .ph = sum_ph / constant::N_SAMPLES,
          .ppm = sum_ppm / constant::N_SAMPLES,
          .level = final_level,
          .temperature = sum_temperature / constant::N_SAMPLES,
          .volume_liter = sensor_manager::get_water_volume_liter(final_level),
      };

      network_manager::mqtt_publish_sensor_data(global_wq);
      state_machine = MEASURE_MORINGA;
      // display_manager::ili9488_println("[STATE MACHINE] State is in MEASURE MORINGA");
    }

    // STATE MACHINE MEASURE MORINGA
    else if (state_machine == MEASURE_MORINGA)
    {
      // display_manager::ili9488_println("[MEASURE MORINGA] AI predict moringa dose and mixing time");
      ai_manager::predict(global_wq);
      global_moringa_dose_gram = ai_manager::get_moringa_dose_gram();
      global_mixing_time_seconds = ai_manager::get_mixing_time_second();
      opening_servo_valve_time_ms = actuator_manager::get_opening_servo_time_by_dose_ms(global_moringa_dose_gram);
      display_manager::state_ai_measure_ui(global_moringa_dose_gram, global_mixing_time_seconds);
      delay(4000);
      // display_manager::ili9488_print("[MEASURE MORING] AI predicted moringa dose ");
      // display_manager::ili9488_println(global_moringa_dose_gram, 2);
      // display_manager::ili9488_print("[MEASURE MORING] AI predicted time mixing ");
      // display_manager::ili9488_println(global_mixing_time_seconds, 2);
      state_machine = DOSING;
      // display_manager::ili9488_println("[STATE MACHINE] State is in DOSING");
    }

    // STATE MACHINE DOSING
    else if (state_machine == DOSING)
    {
      static bool dosing_initialized = false;
      static unsigned long dosing_start_ms = 0;

      if (!dosing_initialized)
      {
        dosing_initialized = true;
        dosing_start_ms = millis();
        actuator_manager::openServoValve();
        display_manager::state_dosing_ui(0.0f,
                                         opening_servo_valve_time_ms / 1000.0f); // draw once immediately
      }

      float elapsed_s = (millis() - dosing_start_ms) / 1000.0f;
      float total_s = opening_servo_valve_time_ms / 1000.0f;

      // Update display every ~1 second
      static float last_display_s = -1.0f;
      if (elapsed_s - last_display_s >= 1.0f)
      {
        last_display_s = elapsed_s;
        display_manager::state_dosing_ui(elapsed_s, total_s);
      }

      // Check if dosing is complete
      if (millis() - dosing_start_ms >= opening_servo_valve_time_ms)
      {
        actuator_manager::closeServoValve();

        // Reset statics for next cycle
        dosing_initialized = false;
        last_display_s = -1.0f;

        state_machine = MIXING;
      }
    }

    // STATE MACHINE MIXING
    else if (state_machine == MIXING)
    {
      static bool mixing_initialized = false;
      static unsigned long mixing_start_ms = 0;

      if (!mixing_initialized)
      {
        mixing_initialized = true;
        mixing_start_ms = millis();
        actuator_manager::turnOnGearMotor();
        display_manager::state_mixing_ui(0.0f,
                                         global_mixing_time_seconds / 1000.0f); // draw once immediately
      }

      float elapsed_s = (millis() - mixing_start_ms) / 1000.0f;
      float total_s = global_mixing_time_seconds / 1000.0f;

      // Update display every ~1 second
      static float last_display_s = -1.0f;
      if (elapsed_s - last_display_s >= 1.0f)
      {
        last_display_s = elapsed_s;
        display_manager::state_mixing_ui(elapsed_s, total_s);
      }

      // Check if mixing is complete
      if (millis() - last_mixing_moringa >= global_mixing_time_seconds)
      {
        last_mixing_moringa = millis();
        actuator_manager::turnOffGearMotor();

        // Reset statics for next cycle
        mixing_initialized = false;
        last_display_s = -1.0f;

        state_machine = FILLING_SETTLING_TANK;
      }
    }

    // STATE MACHINE FILLING SETTLING TANK
    else if (state_machine == FILLING_SETTLING_TANK)
    {
      static bool pump2_started = false;

      if (!pump2_started)
      {
        pump2_started = true;
        actuator_manager::turn_on_pump_2();
      }

      float water_level = sensor_manager::get_distance_ultrasonic_2();

      display_manager::state_filling_settling_tank_ui(water_level);

      if (water_level <= constant::MAX_SETTLING_TANK_LEVEL)
      {
        actuator_manager::turn_off_pump_2();

        // Reset static for next cycle
        pump2_started = false;

        current_time = millis();
        state_machine = SETTLING;
      }
    }

    // STATE MACHINE SETTLING
    else if (state_machine == SETTLING)
    {
      float elapsed_s = (millis() - current_time) / 1000.0f;
      float total_s = constant::SETTLING_TIME / 1000.0f;

      // Publish sensor data at configured interval
      if (millis() - last_publish_settling_data >= constant::PUBLISH_SETTLING_INTERVAL_MS)
      {
        last_publish_settling_data = millis();
        sensor_manager::update_all();

        float distance_2 = sensor_manager::get_distance_ultrasonic_2();
        float temperature_2 = sensor_manager::get_temperature_c_2();
        float ntu_2 = sensor_manager::get_turbidity_ntu_value_2();
        float ph_2 = sensor_manager::get_ph_value_2();
        float ppm_2 = sensor_manager::get_tds_ppm_value_2();
        float volume_2 = sensor_manager::get_water_volume_liter(distance_2);

        WaterQuality water_quality = {
            .ntu = ntu_2,
            .ph = ph_2,
            .ppm = ppm_2,
            .level = distance_2,
            .temperature = temperature_2,
            .volume_liter = volume_2,
        };
        last_settling_wq = water_quality;

        network_manager::mqtt_publish_sensor_data(water_quality);

        // Update display alongside each publish tick
        display_manager::state_settling_ui(water_quality, elapsed_s, total_s);
      }
      else
      {
        // Update timer every second even when not publishing
        static float last_timer_display_s = -1.0f;
        if (elapsed_s - last_timer_display_s >= 1.0f)
        {
          last_timer_display_s = elapsed_s;
          // Reuse last known wq — only timer region redraws (no clear_body)
          display_manager::state_settling_ui(last_settling_wq, elapsed_s, total_s);
        }
      }

      if (millis() - current_time >= constant::SETTLING_TIME)
      {
        state_machine = COMPLETED;
      }
    }

    // STATE MACHINE COMPLETED
    else if (state_machine == COMPLETED)
    {
      display_manager::state_completed_ui();
      network_manager::mqtt_publish_completed();
      // display_manager::ili9488_println("[COMPLETED] Process is completed, go back to IDLE");
      // display_manager::ili9488_println("[COMPLETED] Turn off all actuators");
      actuator_manager::closeServoValve();
      actuator_manager::turn_off_pump_1();
      actuator_manager::turn_off_pump_2();
      actuator_manager::turnOffGearMotor();

      delay(3000);
      state_machine = IDLE;
    }

    // STATE MACHINE FAILED
    else if (state_machine == FAILED)
    {
      // TODO: DISPLAY COMPLETED FAILED
      // display_manager::ili9488_println("[FAILED] Turn off all actuators");
      actuator_manager::closeServoValve();
      actuator_manager::turn_off_pump_1();
      actuator_manager::turn_off_pump_2();
      actuator_manager::turnOffGearMotor();
      state_machine = IDLE;
    }

    // STATE MACHINE OTHER
    else
    {
      // TODO: DISPLAY OTHER UI
      // display_manager::ili9488_println("[STATE MACHINE] Undefine value of state machine, back to idle ");
      state_machine = IDLE;
    }
  }
}