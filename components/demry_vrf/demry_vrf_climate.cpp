#include "demry_vrf_climate.h"

namespace esphome {
  namespace demry_vrf {

    void DemryVrfClimate::setup() {

    }

    void DemryVrfClimate::control(const climate::ClimateCall &call) {

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
