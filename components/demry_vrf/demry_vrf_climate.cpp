#include "demry_vrf_climate.h"

namespace esphome {
  namespace demry_vrf {

    void DemryVrfClimate::setup() {

    }

    void DemryVrfClimate::control(const climate::ClimateCall &call) {
      if (call.get_mode().has_value()) {
        // User requested mode change
        this->mode = *call.get_mode();
      }
      if (call.get_target_temperature().has_value()) {
        this->target_temperature = *call.get_target_temperature();
      }

      if (call.get_fan_mode().has_value()) {
        this->fan_mode = *call.get_fan_mode();
      }

      uint8_t data[10] = {  0x01, this->idx_, 0xFF, 0x04, 0x1D, 0x03, 0xFF, 0xFF, 0xFF, 0x00 };
      if (this->mode == climate::ClimateMode::CLIMATE_MODE_OFF) {
        data[CMD_IDX_ON_OFF] = CMD_OFF;

        uint8_t offData[10] = {  0x01, this->idx_, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA };
        offData[9] = offData[9] + 0x01 + this->idx_;
        this->uart_->write_array(offData, sizeof(offData));
        ESP_LOGD(TAG, "receive control cmd, mode=off, send %x:%x:%x:%x:%x:%x:%x:%x:%x:%x",
                 offData[0], offData[1], offData[2], offData[3], offData[4],
                 offData[5], offData[6], offData[7], offData[8], offData[9]);

        this->publish_state();
        return;

      } else {
        data[CMD_IDX_ON_OFF] = CMD_ON;
        if (this -> mode == climate::ClimateMode::CLIMATE_MODE_COOL) {
          data[CMD_IDX_CLIMATE_MODE] = CMD_CLIMATE_MODE_COOL;
        } else if (this -> mode == climate::ClimateMode::CLIMATE_MODE_HEAT) {
          data[CMD_IDX_CLIMATE_MODE] = CMD_CLIMATE_MODE_HEAT;
        } else if (this -> mode == climate::ClimateMode::CLIMATE_MODE_FAN_ONLY) {
          data[CMD_IDX_CLIMATE_MODE] = CMD_CLIMATE_MODE_FAN_ONLY;
        } else if (this -> mode == climate::ClimateMode::CLIMATE_MODE_DRY) {
          data[CMD_IDX_CLIMATE_MODE] = CMD_CLIMATE_MODE_DRY;
        }
      }

      if (this->fan_mode == climate::CLIMATE_FAN_HIGH) {
        data[CMD_IDX_FAN_MODE] = CMD_FAN_MODE_HIGH;
      } else if (this->fan_mode == climate::CLIMATE_FAN_MEDIUM) {
        data[CMD_IDX_FAN_MODE] = CMD_FAN_MODE_MEDIUM;
      } else if (this->fan_mode == climate::CLIMATE_FAN_LOW) {
        data[CMD_IDX_FAN_MODE] = CMD_FAN_MODE_LOW;
      } else if (this->fan_mode == climate::CLIMATE_FAN_AUTO) {
        data[CMD_IDX_FAN_MODE] = CMD_FAN_MODE_AUTO;
      }

      data[CMD_IDX_TARGET_TEMP] = this->target_temperature;

      data[9] = data[0] + data[1] + data[2] + data[3] + data[4] + data[5] - 0x03;

      this->uart_->write_array(data, sizeof(data));
      ESP_LOGD(TAG, "receive control cmd, send %x:%x:%x:%x:%x:%x:%x:%x:%x:%x",
               data[0], data[1], data[2], data[3], data[4],
               data[5], data[6], data[7], data[8], data[9]);

      this->publish_state();
    }

    climate::ClimateTraits DemryVrfClimate::traits() {
      auto traits = climate::ClimateTraits();
      traits.set_supports_current_temperature(true);
      traits.set_visual_target_temperature_step(1);
      traits.set_visual_temperature_step(1);
      traits.set_visual_min_temperature(16);
      traits.set_visual_max_temperature(30);
      traits.set_supported_fan_modes({
          climate::CLIMATE_FAN_AUTO,
          climate::CLIMATE_FAN_LOW,
          climate::CLIMATE_FAN_MEDIUM,
          climate::CLIMATE_FAN_HIGH
        });
      traits.set_supported_modes({
          climate::CLIMATE_MODE_OFF,
          climate::CLIMATE_MODE_HEAT,
          climate::CLIMATE_MODE_COOL,
          climate::CLIMATE_MODE_FAN_ONLY,
          climate::CLIMATE_MODE_DRY
        });
      return traits;
    }
  }
}
