#include "esphome/core/log.h"
#include "empty_switch.h"

namespace esphome {
namespace empty_switch {

static const char *TAG = "empty_switch.switch";

void EmptySwitch::setup() {
    ESP_LOGCONFIG(TAG, "Setting up custom pio_hbridge-switch...");
    if (!pin_) {
        ESP_LOGW(TAG, "No pin defined for custom pio_hbridge-switch '%s'", this->get_name().c_str());
        return;
    }

    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_OUT);
}

void EmptySwitch::write_state(bool state) {

    if (!pin_) {
        ESP_LOGW(TAG, "No pin defined for custom pio_hbridge-switch '%s'", this->get_name().c_str());
        return;
    }

    gpio_put(pin_, state);
}

void EmptySwitch::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty custom switch");
}

} //namespace empty_switch
} //namespace esphome