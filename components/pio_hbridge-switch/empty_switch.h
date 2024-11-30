#pragma once

#ifdef USE_RP2040

#include "pico/stdlib.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace empty_switch {

class EmptySwitch : public switch_::Switch, public Component {
 public:
  void set_pin(uint8_t pin) { pin_ = pin; }
  void setup() override;
  void write_state(bool state) override;
  void dump_config() override;
 protected:
  uint8_t pin_;
};

} //namespace empty_switch
} //namespace esphome

#endif