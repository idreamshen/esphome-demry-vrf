#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"

namespace esphome {
  namespace demry_vrf {
    class DemryVrfClimate : public Component, public climate::Climate {
    public:
      DemryVrfClimate(uint8_t idx) {
        this->idx_ = idx;
      }
      void setup() override;
      void control(const climate::ClimateCall &call) override;
      climate::ClimateTraits traits() override;
      void set_uart(uart::UARTComponent *uartComponent) {
        this->uart_ = uartComponent;
      }
    protected:
      uart::UARTComponent* uart_;
      uint8_t idx_;
    };
  }
}
