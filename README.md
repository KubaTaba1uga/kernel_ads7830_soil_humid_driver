# ads7830_soil_humid_driver

A Linux kernel module that implements a custom I2C driver for the ADS7830 ADC, enabling soil humidity measurements on up to 8 channels. Each channel’s humidity value is exposed through a dedicated sysfs attribute.

## Building

1. **Install required headers and tools**:
   ```bash
   sudo apt-get install build-essential linux-headers-$(uname -r)
   ```
2. **Build the module**:
   ```bash
   make
   ```

## Usage

1. **Wiring**  
   - Connect ADS7830 to Raspberry Pi 5’s I2C pins (GPIO2 = SDA, GPIO3 = SCL on `i2c-1`).

2. **Device Tree Overlay**  
   Insert into `arch/arm64/boot/dts/broadcom/bcm2712-rpi-5-b.dts`:
   ```dts
   &i2c1 {
       #address-cells = <1>;
       #size-cells = <0>;
       ads7830_soil_humid_device@48 {
           compatible = "raspberrypi,ads7830_soil_humid_device";
           reg = <0x48>;
           status = "okay";
       };
   };
   ```
   Rebuild, update your boot partition, and reboot.

3. **Load / Unload Module**:
   ```bash
   sudo insmod ads7830_soil_humid_driver.ko
   ...
   sudo rmmod ads7830_soil_humid_driver
   ```

4. **Check Humidity in Sysfs** (Raspberry Pi 5 path example):
   ```bash
   cat /sys/devices/platform/axi/1000120000.pcie/1f00074000.i2c/i2c-1/1-0048/humidity_0
   cat /sys/devices/platform/axi/1000120000.pcie/1f00074000.i2c/i2c-1/1-0048/humidity_1
   ...
   ```

## Soil humidity sensor calibration

All capacitive soil humidity sensors which i tried worked using similiar mechanism. In datasheet
producent declares max and min sensor voltage. Closer the voltage is to declared minimum the value
read on sensor is closer to 100% humidity. Similiary for maximum voltage, closer we are the value
goes closer to 0%. For sensor i bougth min was 1V and max was 2.3V. If you do not have datasheet 
you can check this values via voltmeter. To know min voltage just put sensor in glass of water,
to now it's max voltage dry it well and perform measurement.

To adjust sensor calibration we have two lkm variables available:
- INPUT_MIN_V
- INPUT_MAX_V

They can be adjusted from value 0V to value 3.3V.

## To-Do
- Improve error handling and logging.
- Improve path for humidity_X files to be more user friendly.

## License

Dual MIT/GPL
