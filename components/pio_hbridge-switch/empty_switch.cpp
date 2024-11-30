#include "esphome/core/log.h"
#include "empty_switch.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

namespace esphome {
namespace empty_switch {

static const char *TAG = "empty_switch.switch";

#include "hardware/structs/pio.h"

static const uint16_t pio_instructions[] = {
//    pio_encode_pull(false, true),                      // pull, noblock
//    pio_encode_mov(pio_x, pio_osr),                    // mov x, osr
//    pio_encode_jmp_not_x(6),                           // jmp !x, to "disabled" (3rd instruction)
    pio_encode_set(pio_pins, 0b01),                // set pins, 0b01 [3]
    pio_encode_set(pio_pins, 0b10),                // set pins, 0b10 [3]
//    pio_encode_jmp(0),                                 // wrap
//    pio_encode_set(pio_pins, 0b00),                    // set pins, 0b00 (disabled)
//    pio_encode_jmp(0),                                 // jmp to "start"
};


void EmptySwitch::setup() {
    ESP_LOGCONFIG(TAG, "Setting up custom PIO H-bridge switch...");

    if (!pin_) {
        ESP_LOGW(TAG, "No pin defined for custom PIO switch '%s'", this->get_name().c_str());
        return;
    }

    // 1. Initialize the PIO program
    pio_program_t my_pio_program = {
        .instructions = pio_instructions,  // Point to the array of instructions
        .length = sizeof(pio_instructions) / sizeof(uint16_t),  // The number of instructions
        .origin = 0
    };

    // Claim a PIO instance and state machine
    pio_ = pio0;  // Use PIO0 (could also be pio1)
    sm_ = pio_claim_unused_sm(pio_, true);

    // Load the PIO program into the PIO memory
    uint offset = pio_add_program(pio_, &my_pio_program);

    // Configure the PIO program
    pio_sm_config cfg = pio_get_default_sm_config();
    sm_config_set_set_pins(&cfg, pin_, 2);  // Configure output pins: pin_ and pin_ + 1
    //sm_config_set_out_shift(&cfg, false, false, 32);  // Configure shifting options
    sm_config_set_clkdiv(&cfg, 8125);  // Adjust clock divider for desired frequency

    // Initialize the PIO state machine
    pio_sm_init(pio_, sm_, offset, &cfg);
    // Start with the switch off
    this->write_state(false);
    pio_sm_set_enabled(pio_, sm_, true);  // Enable the state machine

    
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
