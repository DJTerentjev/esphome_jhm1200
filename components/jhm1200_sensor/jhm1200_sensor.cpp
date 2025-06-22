#include "jhm1200_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace jhm1200_sensor {

static const char *const TAG = "jhm1200_sensor";

void JHM1200Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up JHM1200 Sensor...");
}

void JHM1200Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "JHM1200 Sensor:");
  LOG_I2C_DEVICE(this);
  if (this->pressure_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Pressure", this->pressure_sensor_);
  }
  if (this->temperature_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  }
  ESP_LOGCONFIG(TAG, "  CAL_L: %.1f Pa", this->cal_l_);
  ESP_LOGCONFIG(TAG, "  CAL_H: %.1f Pa", this->cal_h_);
  ESP_LOGCONFIG(TAG, "  Temperature offset: %.1f", this->temp_offset_);
  ESP_LOGCONFIG(TAG, "  Temperature scale: %.1f", this->temp_scale_);
}

void JHM1200Sensor::update() {
  if (this->read_sensor_data_()) {
    //ESP_LOGD(TAG, "Successfully read sensor data");
  } else {
    ESP_LOGE(TAG, "Failed to read sensor data");
    this->status_set_warning();
  }
}

bool JHM1200Sensor::read_sensor_data_() {
  // Send measurement command
  if (!this->write_bytes(0xAC, nullptr, 0)) {
    ESP_LOGE(TAG, "Failed to send measurement command");
    return false;
  }

  // Wait for measurement to complete
  this->set_timeout(5, [this]() {
    // Check if sensor is busy
    this->set_timeout(300, [this]() {  // Max 100ms wait
      if (this->is_sensor_busy_()) {
        ESP_LOGD(TAG, "Sensor still busy, retrying...");
        this->set_timeout(5, [this]() { this->read_sensor_data_(); });
        return;
      }

      // Read 6 bytes of data
      uint8_t buffer[6];
      if (!this->read_bytes_raw(buffer, 6)) {
        ESP_LOGE(TAG, "Failed to read sensor data");
        this->status_set_warning();
        return;
      }

      // Convert raw data to pressure
      uint32_t press_raw = ((uint32_t) buffer[1] << 16) | ((uint32_t) buffer[2] << 8) | buffer[3];
      uint16_t temp_raw = ((uint16_t) buffer[4] << 8) | buffer[5];
      ESP_LOGD(TAG, "Raw Pressure: %d", press_raw);
      ESP_LOGD(TAG, "Raw Temperature: %d", temp_raw);
      
      // Calculate pressure in kPa
      double press_normalized = (double) press_raw / 16777216.0;
      float pressure_pa = press_normalized * (this->cal_h_ - this->cal_l_) + this->cal_l_;
      float pressure_kpa = pressure_pa / 1000.0f;
      
      // Calculate temperature degC
      double temp = static_cast<double>(temp_raw) / 65535.0;
      float temperature = temp *  this->temp_scale_ +  this->temp_offset_;
      
      // Publish sensor values
      if (this->pressure_sensor_ != nullptr) {
        this->pressure_sensor_->publish_state(pressure_kpa);
      }
      if (this->temperature_sensor_ != nullptr) {
        this->temperature_sensor_->publish_state(temperature);
      }

      this->status_clear_warning();
    });
  });

  return true;
}

bool JHM1200Sensor::is_sensor_busy_() {
  uint8_t status;
  if (!this->read_bytes_raw(&status, 1)) {
    ESP_LOGE(TAG, "Failed to read sensor status");
    return false;
  }
  return (status >> 5) & 0x01;
}



}  // namespace jhm1200_sensor
}  // namespace esphome
