#include <linux/i2c.h>

#include "common.h"
#include "linux/dev_printk.h"
#include "receive_data.h"

static int convert_volts_to_bits(int volts);

int ads7830_soil_humid_receive_data(struct ads7830_soil_humid_data *driver_data,
				    struct ads7830_soil_humid_channel_data *channel_data)
{
	u8 control_byte;
	int err;

	switch (channel_data->channel_number) {
	case 0:
		// Send the control byte for single-ended, channel 0, power-down mode
		control_byte = 0x80;
		break;

	case 1:
		// Send the control byte for single-ended, channel 1, power-down mode
		control_byte = 0xC0;
		break;

	case 2:
		// Send the control byte for single-ended, channel 2, power-down mode
		control_byte = 0x90;
		break;

	case 3:
		// Send the control byte for single-ended, channel 3, power-down mode
		control_byte = 0xD0;
		break;

	case 4:
		// Send the control byte for single-ended, channel 4, power-down mode
		control_byte = 0xA0;
		break;

	case 5:
		// Send the control byte for single-ended, channel 5, power-down mode
		control_byte = 0xE0;
		break;

	case 6:
		// Send the control byte for single-ended, channel 6, power-down mode
		control_byte = 0xB0;
		break;

	case 7:
		// Send the control byte for single-ended, channel 7, power-down mode
		control_byte = 0xF0;
		break;

	default:
		return EINVAL;
	}

	err = i2c_smbus_write_byte(driver_data->client, control_byte);
	if (err) {
		dev_err(&driver_data->client->dev,
			"Unable to write command byte to channel %d\n",
			channel_data->channel_number);
		return err;
	}

	err = i2c_smbus_read_byte(driver_data->client);
	if (err < 0) {
		dev_err(&driver_data->client->dev,
			"Unable to read response from channel %d\n",
			channel_data->channel_number);
		return err;
	}
	// Because sensor return value in bits we need to convert volts also
	//  to bits to bring everything to one unit. Because
	int max_bits = convert_volts_to_bits(driver_data->max_output_voltage -
					     driver_data->min_output_voltage);
	int reading_value = (u8) err - convert_volts_to_bits(driver_data->min_output_voltage);

	int reversed_reading_value = max_bits - reading_value;

	dev_dbg(&driver_data->client->dev,
		"max=%d, value=%d, reversed_value=%d, "
		"convert_volts_to_bits(driver_data->min_output_voltage)=%d\n",
		max_bits, reading_value, reversed_reading_value,
		convert_volts_to_bits(driver_data->min_output_voltage));

	// i2c_smbus_read_byte returns data on success
	//  there is no float in kernel so we are ensuring
	//  that we end up with int between 0 and 100
	//  presenting data in procentage unit.
	channel_data->humidity = (reversed_reading_value * 100) / max_bits;

	if (channel_data->humidity > 100) {
		channel_data->humidity = 100;
	} else if (channel_data->humidity < 0) {
		channel_data->humidity = 0;
	}

	return 0;
}

// We are assuming here that `volts` arg is already rescaled.
static int convert_volts_to_bits(int volts)
{
	int volts_per_bit;

	// This is how much rescaled volts one bit represent
	volts_per_bit = ads7830_soil_humid_rescale_volts(MAX_VOLTAGE) / 255;

	return volts / volts_per_bit;
}
