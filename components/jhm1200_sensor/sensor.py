import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    CONF_TEMPERATURE,
    CONF_PRESSURE,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    ICON_THERMOMETER,
    DEVICE_CLASS_PRESSURE,
    ICON_GAUGE,
)

DEPENDENCIES = ["i2c"]

CODEOWNERS = ["@your_username"]

jhm1200_sensor_ns = cg.esphome_ns.namespace("jhm1200_sensor")
JHM1200Sensor = jhm1200_sensor_ns.class_(
    "JHM1200Sensor", cg.PollingComponent, i2c.I2CDevice
)

CONF_CAL_L = "cal_l"
CONF_CAL_H = "cal_h"
CONF_TEMP_OFFSET = "temp_offset"
CONF_TEMP_SCALE = "temp_scale"


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(JHM1200Sensor),
            cv.Optional(CONF_PRESSURE): sensor.sensor_schema(
                unit_of_measurement="kPa",
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_PRESSURE,
                state_class=STATE_CLASS_MEASUREMENT,
                icon=ICON_GAUGE,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
                icon=ICON_THERMOMETER,
            ),
            cv.Optional(CONF_CAL_L, default=-125000.0): cv.float_,
            cv.Optional(CONF_CAL_H, default=1125000.0): cv.float_,
            cv.Optional(CONF_TEMP_OFFSET, default=-4000): cv.float_,
            cv.Optional(CONF_TEMP_SCALE, default=19000): cv.float_,
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x78))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_PRESSURE in config:
        sens = await sensor.new_sensor(config[CONF_PRESSURE])
        cg.add(var.set_pressure_sensor(sens))

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))

    cg.add(var.set_cal_l(config[CONF_CAL_L]))
    cg.add(var.set_cal_h(config[CONF_CAL_H]))
    cg.add(var.set_temp_offset(config[CONF_TEMP_OFFSET]))
    cg.add(var.set_temp_scale(config[CONF_TEMP_SCALE]))
