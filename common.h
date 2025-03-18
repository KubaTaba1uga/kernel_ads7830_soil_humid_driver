#ifndef ADS7830_SOIL_HUMID_COMMON
#define ADS7830_SOIL_HUMID_COMMON
#include <linux/kobject.h>
#include <linux/platform_device.h>
#include <linux/types.h>

struct ads7830_soil_humid_channel_data {
  int channel_number;
  int humidity;
};

struct ads7830_soil_humid_data {
  struct mutex access_lock;
  struct i2c_client *client;
  // ADS7830 has eigth channels, so we are creating one attribute for each
  //   channel. This way we will have 8 entries in sysfs each for one sensor.
  struct device_attribute dev_attrs[8];
  // channels_data is used to store data received from all channels.
  struct ads7830_soil_humid_channel_data channels_data[8];
};

#define LKM_PRINT_ERR(client, fmt, ...)                                        \
  dev_err(&client->dev, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#endif
