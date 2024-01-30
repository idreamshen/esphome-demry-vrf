#include "demry_vrf_component.h"

namespace esphome {
  namespace demry_vrf {
    void DemryVrfComponent::setup() {

    }

    void DemryVrfComponent::update() {
      if (this->update_pos_ >= this->climates_.size()) {
        this->update_pos_ = 0;
      }
      byte data[10] = {  0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA };
      data[1] = data[1] + this->climates_[this->update_pos_]->get_idx();
      data[9] = data[9] + this->climates_[this->update_pos_]->get_idx();
      write_array(data, sizeof(data));
      ESP_LOGD(TAG, "update loop, send %x:%x:%x:%x:%x:%x:%x:%x:%x:%x",
               data[0], data[1], data[2], data[3], data[4],
               data[5], data[6], data[7], data[8], data[9]);

      this->update_pos_++;
    }

    void DemryVrfComponent::loop() {
      while(available() >= 10) {
        uint8_t data[10];
        for (int i = 0; i <= 9; i++) {
          uint8_t c = read();
          data[i] = c;
        }

        ESP_LOGD(TAG, "uart bus receive %x:%x:%x:%x:%x:%x:%x:%x:%x:%x",
                 data[0], data[1], data[2], data[3], data[4],
                 data[5], data[6], data[7], data[8], data[9]);

        uint8_t sum = 0;
        for (int i = 0; i <= 8; i++) {
          sum += data[i];
        }

        if (sum != data[9]) {
          ESP_LOGD(TAG, "checksum fail, sum=%d, data[9]=%d", sum, data[9]);
          read(); // drop one

          continue;
        } else {
          ESP_LOGD(TAG, "checksum succ, sum=%d", sum);
        }

        uint8_t identity = data[CMD_IDX_IDENTITY];
        ESP_LOGD(TAG, "identity=%d receive data", identity);

        DemryVrfClimate* demryClimate = nullptr;
        for(auto &climate : this->climates_) {
          if (climate->get_idx() == identity) {
            demryClimate = climate;
          }
        }

        if (demryClimate == nullptr) {
          ESP_LOGW(TAG, "identity=%d cannot find climate", identity);
          return;
        }

        if (data[CMD_IDX_ON_OFF] == CMD_OFF) {
          ESP_LOGD(TAG, "identity=%d is off", identity);
          demryClimate->mode = climate::ClimateMode::CLIMATE_MODE_OFF;
        } else if (data[CMD_IDX_ON_OFF] == CMD_ON) {
          ESP_LOGD(TAG, "identity=%d is on", identity);
          if (data[CMD_IDX_CLIMATE_MODE] == CMD_CLIMATE_MODE_COOL) {
            ESP_LOGD(TAG, "identity=%d climate_mode is cool", identity);
            demryClimate->mode = climate::ClimateMode::CLIMATE_MODE_COOL;
          } else if (data[CMD_IDX_CLIMATE_MODE] == CMD_CLIMATE_MODE_HEAT) {
            ESP_LOGD(TAG, "identity=%d climate_mode is heat", identity);
            demryClimate->mode = climate::ClimateMode::CLIMATE_MODE_HEAT;
          } else if (data[CMD_IDX_CLIMATE_MODE] == CMD_CLIMATE_MODE_FAN_ONLY) {
            ESP_LOGD(TAG, "identity=%d climate_mode is fan_only", identity);
            demryClimate->mode = climate::ClimateMode::CLIMATE_MODE_FAN_ONLY;
          } else if (data[CMD_IDX_CLIMATE_MODE] == CMD_CLIMATE_MODE_DRY) {
            ESP_LOGD(TAG, "identity=%d climate_mode is dry", identity);
            demryClimate->mode = climate::ClimateMode::CLIMATE_MODE_DRY;
          }
        }

        if (data[CMD_IDX_FAN_MODE] == CMD_FAN_MODE_AUTO) {
          ESP_LOGD(TAG, "identity=%d fan_mode is auto", identity);
          demryClimate->fan_mode = climate::CLIMATE_FAN_AUTO;
        } else if (data[CMD_IDX_FAN_MODE] == CMD_FAN_MODE_LOW) {
          ESP_LOGD(TAG, "identity=%d fan_mode is low", identity);
          demryClimate->fan_mode = climate::CLIMATE_FAN_LOW;
        } else if (data[CMD_IDX_FAN_MODE] == CMD_FAN_MODE_MEDIUM) {
          ESP_LOGD(TAG, "identity=%d fan_mode is medium", identity);
          demryClimate->fan_mode = climate::CLIMATE_FAN_MEDIUM;
        } else if (data[CMD_IDX_FAN_MODE] == CMD_FAN_MODE_HIGH) {
          ESP_LOGD(TAG, "identity=%d fan_mode is high", identity);
          demryClimate->fan_mode = climate::CLIMATE_FAN_HIGH;
        }

        demryClimate->target_temperature = data[CMD_IDX_TARGET_TEMP];
        demryClimate->current_temperature = data[CMD_IDX_CURRENT_TEMP];

        demryClimate->publish_state();
      }
    }

  }
}
