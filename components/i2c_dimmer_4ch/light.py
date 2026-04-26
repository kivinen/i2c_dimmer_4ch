import esphome.codegen as cg
from esphome.components import light, output
from esphome.components import i2c
from esphome.components.i2c import I2CBus
import esphome.config_validation as cv
from esphome.components.light import  BRIGHTNESS_ONLY_LIGHT_SCHEMA
from esphome.const import (
    CONF_OUTPUT_ID,
    CONF_CHANNEL,
    CONF_MIN_BRIGHTNESS,
    CONF_MAX_BRIGHTNESS, 
)

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

i2c_dimmer_4ch_ns = cg.esphome_ns.namespace("i2c_dimmer_4ch")
I2CDimmer4ChLightOutput = i2c_dimmer_4ch_ns.class_("I2CDimmer4ChLightOutput",
                                                   light.LightOutput)

CONFIG_SCHEMA = (
    BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend(
        {
            cv.Required(CONF_CHANNEL): cv.hex_int_range(0, 255),
        }))

#light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend(
#        {
#            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(I2CDimmer4ChLightOutput),
#            cv.Required(CONF_CHANNEL): cv.hex_int_range(0, 255),
#            cv.Optional(CONF_MIN_BRIGHTNESS, default=0): cv.uint8_t,
#            cv.Optional(CONF_MAX_BRIGHTNESS, default=100): cv.uint8_t,
#        }
#    )
#    .extend(cv.COMPONENT_SCHEMA)
#    .extend(i2c.i2c_device_schema(0x3f))
#)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
#    await light.register_light(var, config)
#
#    brightness = await cg.get_variable(config[CONF_BRIGHTNESS])
#    cg.add(var.set_brightness(brightness))
#    cg.add(var.set_min_brightness(config[CONF_MIN_BRIGHTNESS]))
#    cg.add(var.set_max_brightness(config[CONF_MAX_BRIGHTNESS]))
#    cg.add(var.set_channel(config[CONF_CHANNEL]))

