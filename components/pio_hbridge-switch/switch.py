DEPENDENCIES = ["rp2040", "rp2040_pio"]

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import switch
from esphome.const import CONF_ID, CONF_PIN

# Define the namespace and class
empty_switch_ns = cg.esphome_ns.namespace('empty_switch')
EmptySwitch = empty_switch_ns.class_('EmptySwitch', switch.Switch, cg.Component)

# Configuration schema
CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
    cv.GenerateID(): cv.declare_id(EmptySwitch)
}).extend(cv.COMPONENT_SCHEMA)

# Convert configuration to code
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    pin = await cg.gpio_pin_expression(config[CONF_PIN])  # Await the coroutine
    cg.add(var.set_pin(pin.get_pin()))
    await cg.register_component(var, config)  # Await registration
    await switch.register_switch(var, config)  # Await switch registration
