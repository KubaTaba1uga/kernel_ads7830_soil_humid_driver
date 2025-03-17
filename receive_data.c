#include <linux/i2c.h>

#include "common.h"
#include "receive_data.h"

int ads7830_soil_humid_receive_data(
    struct ads7830_soil_humid_data *driver_data,
    struct ads7830_soil_humid_channel_data *channel_data) {
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

  // i2c_smbus_read_byte returns data on success
  //  there is no float in kernel so we are ensuring
  //  that we end up with int between 0 and 100
  //  presenting data in procentage unit.
  channel_data->humidity = ((u8)err * 100) / 255;

  return 0;
}
