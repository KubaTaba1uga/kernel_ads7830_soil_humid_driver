/*
 * ads7830_soil_humid driver
 ****************************************************************
 * Brief Description:
 * A simple module which implements driver for ADS7830 and implements coversion
 *  for simple soil humid driver. Each channel is occupied by one soild humid
 *  sensor. Sensors values can be accessed under sysfs.
 */
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/moduleparam.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

#include "common.h"
#include "init_ads7830.h"
#include "sysfs.h"

static int mp_max_voltage_output;
static int mp_min_voltage_output;
module_param(mp_max_voltage_output, int, 0);
MODULE_PARM_DESC(mp_max_voltage_output, "Maximum sensor voltage read, 33 means 3.3V [0-33]");

module_param(mp_min_voltage_output, int, 0);
MODULE_PARM_DESC(mp_min_voltage_output, "Minimum sensor voltage read, 33 means 3.3V [0-33]");

#define IS_MP_VOLTAGE_INVALID(var) (var > 33 || var < 0)
static int validate_module_params(struct i2c_client *client);

static int ads7830_soil_humid_probe(struct i2c_client *client)
{
	struct ads7830_soil_humid_data *data;
	int err;

	dev_info(&client->dev, "Probing...\n");

	err = validate_module_params(client);
	if (err) {
		return err;
	}

	err = ads7830_soil_humid_init(client);
	if (err) {
		LKM_PRINT_ERR(client, "Unable to init ads7830\n");
		return err;
	}

	data = i2c_get_clientdata(client);
	data->min_output_voltage = ads7830_soil_humid_rescale_volts(mp_min_voltage_output);
	data->max_output_voltage =
	    ads7830_soil_humid_rescale_volts(mp_max_voltage_output !=
					     0 ? mp_max_voltage_output : MAX_VOLTAGE);

	err = ads7830_soil_humid_init_sysfs(data);
	if (err) {
		LKM_PRINT_ERR(client, "Unable to init sysfs\n");
		return err;
	}

	dev_info(&client->dev, "ADS7830_soil_humid_driver probed\n");

	return 0;
}

static void ads7830_soil_humid_remove(struct i2c_client *client)
{
	struct ads7830_soil_humid_data *data;

	data = i2c_get_clientdata(client);

	ads7830_soil_humid_destroy_sysfs(data);

	dev_info(&client->dev, "ADS7830_soil_humid_driver removed\n");
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

static struct i2c_driver ads7830_soil_humid_driver = {
	.probe = ads7830_soil_humid_probe,
	.remove = ads7830_soil_humid_remove,
	.driver = {
		   .name = "ads7830_soil_humid_i2c",
		   .of_match_table = ads7830_soil_humid_of_match,
		   },

};

module_i2c_driver(ads7830_soil_humid_driver);

MODULE_AUTHOR("Jakub Buczynski");
MODULE_DESCRIPTION("Custom soil humidity driver utilising ADS7830");
MODULE_LICENSE("Dual MIT/GPL");

static int validate_module_params(struct i2c_client *client)
{
	if (IS_MP_VOLTAGE_INVALID(mp_min_voltage_output)) {
		LKM_PRINT_ERR(client, "Invalid mp_min_voltage_output\n");
		return -EINVAL;
	}

	if (IS_MP_VOLTAGE_INVALID(mp_max_voltage_output)) {
		LKM_PRINT_ERR(client, "Invalid mp_max_voltage_output\n");
		return -EINVAL;
	}

	return 0;
}
