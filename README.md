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

## To-Do
- Improve error handling and logging.
- Improve path for humidity_X files to be more user friendly.

## License

Dual MIT/GPL
