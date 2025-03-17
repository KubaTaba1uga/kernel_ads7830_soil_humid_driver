# am2303

Main docs regarding i2c for kernel is here i2c/index.html.

We need to edit arch/arm64/boot/dts/broadcom/bcm2712-rpi-5-b.dts to tell kernel what device we expect on which i2c bus. We will connect our ADS7830 to GPIOs 2 and 3 which stands for bus number 1
```
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
