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
  int min_output_voltage;
  int max_output_voltage;
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

// We are assuming here that maximum ADC channel reading is 3.3V.
//  This is because we connect ADC to 3.3V, if somehow that's not
//  your case and your ADC shows maximum different value, you may
//  be interested in changing this macro.
#define MAX_VOLTAGE 33

// We take volts from user as double digit (33 is 3.3V) but it is easier to
//  operate on rescaled value by 1k, so we do not need to deal with floats.
static inline int ads7830_soil_humid_rescale_volts(int volts) {
  return volts * 1000;
}

#endif
