import esphome.codegen as cg
from esphome.components import light, output 
from esphome.components import i2c
from esphome.components.i2c import I2CBus
import esphome.config_validation as cv
from esphome.const import (
    CONF_CHANNEL,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_OUTPUT_ID,
)

DEPENDENCIES = ["i2c"]

i2c_dimmer_4ch_ns = cg.esphome_ns.namespace("i2c_dimmer_4ch")
I2CDimmer4ChLightOutput = i2c_dimmer_4ch_ns.class_("I2CDimmer4ChLightOutput",
                                                   light.LightOutput,
                                                   output.FloatOutput,
                                                   cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = (
    light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(I2CDimmer4ChLightOutput),
            cv.Required(CONF_CHANNEL): cv.hex_int_range(0, 255),
            cv.Optional(CONF_MIN_VALUE, default=0): cv.uint8_t,
            cv.Optional(CONF_MAX_VALUE, default=100): cv.uint8_t,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x3f))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    
    cg.add(var.set_min_value(config[CONF_MIN_VALUE]))
    cg.add(var.set_max_value(config[CONF_MAX_VALUE]))
    cg.add(var.set_channel(config[CONF_CHANNEL]))

