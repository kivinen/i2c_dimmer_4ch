#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/output/float_output.h"

#define TAG "i2c_dimmer_4ch"

namespace esphome {
  namespace i2c_dimmer_4ch {
    
    class I2CDimmer4ChLightOutput : public Component,
                                    public i2c::I2CDevice,
                                    public light::LightOutput {
    public:
      void set_min_value(uint8_t min_value) {
        ESP_LOGD(TAG, "Setting min value to %d", min_value);
        min_value_ = min_value;
      }
      void set_max_value(uint8_t max_value) {
        ESP_LOGD(TAG, "Setting max value to %d", max_value);
        max_value_ = max_value;
      }
      void set_channel(uint8_t channel) {
        ESP_LOGD(TAG, "Setting channel to %d", channel);
        channel_ = channel; }
      
      light::LightTraits get_traits() override {
        auto traits = light::LightTraits();
        traits.set_supported_color_modes({light::ColorMode::BRIGHTNESS});
        return traits;
      }
      
      void write_state(light::LightState *state) override {
        float brightness;
        uint8_t brightness_int;
        
        state->current_values_as_brightness(&brightness);

        if (brightness < 0.001f) {
          brightness_int = 100;
        } else {
          brightness_int =
            remap<uint8_t, float>(brightness, 0.0f, 1.0f,
                                  max_value_, min_value_);
        }
        if (brightness_int == brightness_) {
          ESP_LOGV(TAG, "Not sending unchanged value");
          return;
        }
        ESP_LOGV(TAG, "Brightness to ch %d update: %d (raw: %f)", channel_,
                 brightness_int, brightness);
        this->send_brightness_(brightness_int);
      }
      
    protected:
      uint8_t brightness_{255};
      uint8_t min_value_{0};
      uint8_t max_value_{100};
      uint8_t channel_{0};

      void send_brightness_(uint8_t brightness) {
        if (!this->write_byte(channel_, brightness)) {
          ESP_LOGE(TAG, "Write brightness %d to channel %d failed",
                   brightness, channel_);
        }
        brightness_ = brightness;
      }
    };
  }  // namespace i2c_dimmer_4ch
}  // namespace esphome

