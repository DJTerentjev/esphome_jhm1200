#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace jhm1200_sensor {

class JHM1200Sensor : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_pressure_sensor(sensor::Sensor *pressure_sensor) { pressure_sensor_ = pressure_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_cal_l(float cal_l) { cal_l_ = cal_l; }
  void set_cal_h(float cal_h) { cal_h_ = cal_h; }
  void set_temp_offset(float temp_offset) { temp_offset_ = temp_offset; }
  void set_temp_scale(float temp_scale) { temp_scale_ = temp_scale; }

  void setup() override;
  void dump_config() override;
  void update() override;

 protected:
  bool read_sensor_data_();
  bool is_sensor_busy_();
  float calculate_temperature_(uint16_t raw_temp);

  sensor::Sensor *pressure_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};

  float cal_l_{-125000.0f};    // Lower calibration limit (Pa)
  float cal_h_{1125000.0f};    // Upper calibration limit (Pa)
  float temp_offset_{-4000};     // Temperature calibration offset
  float temp_scale_{19000};     // Temperature calibration scale

};

}  // namespace jhm1200_sensor
}  // namespace esphome
