#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"

#define TAG "demry.vrf"

#define CMD_IDX_IDENTITY 1
#define CMD_IDX_ON_OFF 2
#define CMD_IDX_CLIMATE_MODE 3
#define CMD_IDX_TARGET_TEMP 4
#define CMD_IDX_FAN_MODE 5
#define CMD_IDX_CURRENT_TEMP 6

#define CMD_ON 0x01
#define CMD_OFF 0x00
#define CMD_CLIMATE_MODE_COOL 0x02
#define CMD_CLIMATE_MODE_HEAT 0x01
#define CMD_CLIMATE_MODE_FAN_ONLY 0x04
#define CMD_CLIMATE_MODE_DRY 0x08
#define CMD_FAN_MODE_AUTO 0x00
#define CMD_FAN_MODE_LOW 0x01
#define CMD_FAN_MODE_MEDIUM 0x02
#define CMD_FAN_MODE_HIGH 0x03

namespace esphome {
  namespace demry_vrf {
    class DemryVrfClimate : public Component, public climate::Climate {
    public:
      void setup() override;
      void control(const climate::ClimateCall &call) override;
      climate::ClimateTraits traits() override;
      void set_uart(uart::UARTComponent *uartComponent) {
        this->uart_ = uartComponent;
      }
    protected:
      uart::UARTComponent* uart_;
    };

    class DemryVrfComponent : public PollingComponent, public uart::UARTDevice {
    public:
      DemryVrfComponent(uart::UARTComponent *uartComponent) : UARTDevice(uartComponent) {
        this->uart_ = uartComponent;
      };
      void setup() override;
      void update() override;
      void loop() override;
      void register_climate(demry_vrf::DemryVrfClimate *obj) {
        this->climates_.push_back(obj);
      }
    protected:
      uart::UARTComponent* uart_;
      std::vector<demry_vrf::DemryVrfClimate *> climates_;
    };
  }
}
