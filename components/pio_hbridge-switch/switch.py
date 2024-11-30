import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import switch
from esphome.const import CONF_ID, CONF_PIN

empty_switch_ns = cg.esphome_ns.namespace('empty_switch')
EmptySwitch = empty_switch_ns.class_('EmptySwitch', switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
    cv.GenerateID(): cv.declare_id(EmptySwitch)
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    pin = cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    cg.register_component(var, config)
    switch.register_switch(var, config)
