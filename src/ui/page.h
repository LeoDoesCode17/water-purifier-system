#pragma once

class Page {
  public:
  virtual ~Page() = default;  
  virtual void render() = 0;
  virtual void on_encoder_move(long delta) = 0; // delta = new_position - old_position
  virtual void on_button_pressed() = 0;
  virtual void on_enter() {}
};