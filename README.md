# esphome_jhm1200
ESPHome External Component for JHM1200 (KY-I2C-3V3) I2C pressure sensor 

![jhm1200](https://github.com/user-attachments/assets/dfc820e4-5c6c-4f2e-9817-a775c39b0740)
## How to use
```yaml
external_components:
  - source: github://DJTerentjev/esphome_jhm1200
    components: [jhm1200_sensor]
```

```yaml
sensor:
  - platform: jhm1200_sensor
    temperature:
      name: "JHM1200 Temperature"
      id: jhm1200_temp
    pressure:
      name: "JHM1200 Pressure"
      id: jhm1200_pressure
    update_interval: 10s
    # Optional calibration parameters (default values shown)
    cal_l: -125000.0    # Lower calibration limit (Pa)
    cal_h: 1125000.0    # Upper calibration limit (Pa)
    temp_offset: -4000   # Temperature calibration offset
    temp_scale: 19000   # Temperature calibration scale
```
## Optional calibration parameters
#Pressure (manufacturer exaple code)
```yaml
cal_l: -125000  #0-1.0 MPA
cal_h: 1125000  #0-1.0 MPA

cal_l: -200000  #0-1.6 MPA
cal_h: 1800000  #0-1.6 MPA
```

#Temperature (manufacturer exaple code dosn't work correctly for my sensor)
```yaml
temp_offset: -4000   # Temperature calibration offset
temp_scale: 19000   # Temperature calibration scale
```
#Temperature coefficients calculated for linear function (work for my sensor)
```yaml
temp_offset: 157.8   # Temperature calibration offset
temp_scale: -226.8   # Temperature calibration scale
```

## Local Installation
Copy files from components/jhm1200_sensor/  File structure:
```
esphome
├──components
│   ├── jhm1200_sensor
│   │   ├── __init.py__
│   │   ├── jhm1200_sensor.cpp
│   │   ├── jhm1200_sensor.h
│   │   ├── sensor.py
```

```yaml
external_components:
  - source: components
    components: [ jhm1200_sensor ]
```
