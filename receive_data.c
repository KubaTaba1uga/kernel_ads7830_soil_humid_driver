#include <linux/i2c.h>

#include "common.h"
#include "receive_data.h"

int ads7830_soil_humid_receive_data(
    struct ads7830_soil_humid_data *driver_data,
    struct ads7830_soil_humid_channel_data *channel_data) {
  int err;

  // Send the control byte for single-ended, channel 0, power-down mode
  err = i2c_smbus_write_byte(driver_data->client, 0b10000000);
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
  channel_data->humidity = (u8)err;

  return 0;
}
