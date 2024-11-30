#include "esphome/core/log.h"
#include "empty_switch.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Include the generated header from the .pio file
#include "hbridge.pio.h"

namespace esphome {
namespace empty_switch {

static const char *TAG = "empty_switch.switch";

void EmptySwitch::setup() {
    ESP_LOGCONFIG(TAG, "Setting up custom PIO H-bridge switch...");

    if (!pin_) {
        ESP_LOGW(TAG, "No pin defined for custom PIO switch '%s'", this->get_name().c_str());
        return;
    }

    // Claim a PIO instance and state machine
    pio_ = pio0;  // Use PIO0 (or pio1 for the second PIO)
    sm_ = pio_claim_unused_sm(pio_, true);

    // Load the program into the PIO
    uint offset = pio_add_program(pio_, &hbridge);

    // Configure the state machine
    pio_sm_config cfg = hbridge_program_get_default_config(offset);
    sm_config_set_set_pins(&cfg, pin_, 2);  // Use pin_ and pin_ + 1 for outputs
    sm_config_set_clkdiv(&cfg, 8125);  // Adjust clock divider for desired frequency

    // Initialize the state machine
    pio_sm_init(pio_, sm_, offset, &cfg);
    pio_sm_set_enabled(pio_, sm_, true);

    // Start with the switch off
    this->write_state(false);
}

void EmptySwitch::write_state(bool state) {
    if (!pin_) {
        ESP_LOGW(TAG, "No pin defined for custom PIO switch '%s'", this->get_name().c_str());
        return;
    }

    // Write the state to the PIO state machine
    pio_sm_put_blocking(pio_, sm_, state ? 1 : 0);

    // Update ESPHome switch state
    //this->publish_state(state);
}

void EmptySwitch::dump_config() {
    ESP_LOGCONFIG(TAG, "Empty custom PIO H-bridge switch:");
    ESP_LOGCONFIG(TAG, "  Pin: %d", this->pin_);
}

}  // namespace empty_switch
}  // namespace esphome
