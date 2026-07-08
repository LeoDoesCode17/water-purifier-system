#include "rotary_encoder.h"
#include <Arduino.h>
#include <RotaryEncoder.h>
#include "config/pins.h"

namespace {
  RotaryEncoder encoder(ENCODER_DT_PIN, ENCODER_CLK_PIN, RotaryEncoder::LatchMode::TWO03);
  namespace rotary_encoder {
    long get_posititon() {
      // Implementation for retrieving the current count of the rotary encoder
      return encoder.getPosition(); // Placeholder return value
    }
    long get_direction() {
      return (long)encoder.getDirection();
    }
  }
}