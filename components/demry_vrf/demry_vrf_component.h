#pragma once

namespace esphome {
  namespace demry_vrf {
    class DemryVrfClimate; // Forward declaration
  }
}
#include "demry_vrf_climate.h"

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"


#define TAG "demry.vrf"

namespace esphome {
  namespace demry_vrf {
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
