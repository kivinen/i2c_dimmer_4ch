#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
  namespace i2c_dimmer_4ch {
    
    class I2CDimmer4ChLightOutput : public Component,
				    public i2c::I2CDevice,
				    public light::LightOutput {
    public:
      void set_brightness(output::FloatOutput *brightness) { brightness_ = brightness; }
      void set_min_brightness(uint8_t min_brightness) { this->min_brightness_ = min_brightness; }
      void set_max_brightness(uint8_t max_brightness) { this->max_brightness_ = max_brightness; }
      void set_channel(uint8_t channel) { this->channel_; }
      
      light::LightTraits get_traits() override {
	auto traits = light::LightTraits();
	traits.set_supported_color_modes({light::ColorMode::BRIGHTNESS});
	return traits;
      }
      
      void write_state(light::LightState *state) override {
	float brightness;
	state->current_values_as_brightness(&brightness);

	if (brightness == 0.0) {
	} else {
	  const uint8_t brightness_int =
	    remap<uint8_t, float>(brightness, 0.0f, 1.0f,
				  this->min_brightness_, this->max_brightness_);
	  if (brightness_int == this->brightness_) {
	    ESP_LOGV(TAG, "Not sending unchanged value");
	    return;
	  }
	  ESP_LOGD(TAG, "Brightness update: %d (raw: %f)", brightness_int, brightness);
	
	  this->send_brightness_(brightness_int);
	} else {
	  this->send_brightness_(0);
	}
      }
      
    protected:
      uint8_t brightness_;
      uint8_t min_brightness_{0};
      uint8_t max_brightness_{100};
      uint8_t channel_{0};

      void send_brightness_(uint8_t brightness) {
	ErrorCode error = this->write_byte(channel_, brightness);
	if (error != ESP_OK) {
	  ESP_LOGE(TAG, "Write brightness %d to channel %d failed", brightness, channel_);
	}
      }
    };
  }  // namespace i2c_dimmer_4ch
}  // namespace esphome
