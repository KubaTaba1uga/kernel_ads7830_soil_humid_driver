/*
 * ads7830_soil_humid driver
 ****************************************************************
 * Brief Description:
 * A simple module which implements driver for ADS7830 and implements coversion
 *  for simple soil humid driver. Each channel is occupied by one soild humid
 *  sensor. Sensors values can be accessed under sysfs.
 */
#include <linux/of_device.h>
#include <linux/platform_device.h>

static int ads7830_soil_humid_probe(struct platform_device *pdev) {
  dev_info(&pdev->dev, "Probing...\n");

  dev_info(&pdev->dev, "ADS7830_soil_humid_driver probed\n");

  return 0;
}

static void ads7830_soil_humid_remove(struct platform_device *pdev) {
  dev_info(&pdev->dev, "ADS7830_soil_humid_driver removed\n");
}

static const struct of_device_id ads7830_soil_humid_of_match[] = {
    {
        // This is a unique value which should match `compatibile` field in
        // overlay.
        .compatible = "raspberrypi,ads7830_soil_humid_device",
    },
    {},
};

MODULE_DEVICE_TABLE(of, ads7830_soil_humid_of_match);

static struct platform_driver ads7830_soil_humid_driver = {
    .probe = ads7830_soil_humid_probe,
    .remove_new = ads7830_soil_humid_remove,
    .driver =
        {
            .name = "ads7830_soil_humid_driver",
            .of_match_table = ads7830_soil_humid_of_match,
        },
};

module_platform_driver(ads7830_soil_humid_driver);

MODULE_AUTHOR("Jakub Buczynski");
MODULE_DESCRIPTION("Custom soil humidity driver utilising ADS7830");
MODULE_LICENSE("GPL");
