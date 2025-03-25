#include <linux/dev_printk.h>
#include <linux/device.h>
#include <linux/gfp_types.h>
#include <linux/i2c.h>
#include <linux/ktime.h>
#include <linux/mutex.h>

#include "common.h"
#include "receive_data.h"
#include "sysfs.h"

#define FOR_EACH_DEV_ATTR(data, func)                                          \
  do {                                                                         \
    for (int _i = 0;                                                           \
         _i < sizeof(data->dev_attrs) / sizeof(struct device_attribute);       \
         _i++) {                                                               \
      struct device_attribute *_dev_attr = &data->dev_attrs[_i];               \
      func;                                                                    \
    }                                                                          \
  } while (0)

static ssize_t show_humidity(struct device *dev, struct device_attribute *attr, char *buf);

int ads7830_soil_humid_init_sysfs(struct ads7830_soil_humid_data *data)
{
	int name_max = 32;
	char *name_buf;
	int err;

	FOR_EACH_DEV_ATTR(data, {
			  name_buf =
			  devm_kzalloc(&data->client->dev, name_max * sizeof(char),
				       GFP_KERNEL); if (!name_buf) {
			  return -ENOMEM;}

			  snprintf(name_buf, name_max, "humidity_%i", _i);
			  _dev_attr->attr.name = name_buf;
			  _dev_attr->attr.mode = 0444;
			  _dev_attr->show = show_humidity;
			  err = device_create_file(&data->client->dev, _dev_attr); if (err) {
			  LKM_PRINT_ERR(data->client, "Unable to create device file for %s",
					name_buf); return err;}
			  }
	) ;

	mutex_init(&data->access_lock);

	dev_set_drvdata(&data->client->dev, data);

	return 0;
};

void ads7830_soil_humid_destroy_sysfs(struct ads7830_soil_humid_data *data)
{
	FOR_EACH_DEV_ATTR(data, {
			  device_remove_file(&data->client->dev, _dev_attr);
			  });
};

static ssize_t show_humidity(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct ads7830_soil_humid_channel_data *channel = NULL;
	struct ads7830_soil_humid_data *data;
	int err = 0;

	data = dev_get_drvdata(dev);

	mutex_lock(&data->access_lock);

	// Find channel
	FOR_EACH_DEV_ATTR(data, {
			  if (_dev_attr == attr) {
			  channel = &data->channels_data[_i];
			  channel->channel_number = _i; channel->humidity = -1; break;};}
	) ;

	if (channel == NULL) {
		LKM_PRINT_ERR(data->client, "Unable to find corresponding ADC channel\n");
		err = ENOENT;
		goto cleanup;
	}

	err = ads7830_soil_humid_receive_data(data, channel);
	if (err) {
		LKM_PRINT_ERR(data->client, "Unable to receive data for %s\n",
			      attr->attr.name);
		goto cleanup;
	}

	err = snprintf(buf, 32, "%d\n", channel->humidity);
	if (err < 0) {
		LKM_PRINT_ERR(data->client, "Unable to write data to sysfs buffer for %s\n",
			      attr->attr.name);
		goto cleanup;
	}

 cleanup:
	mutex_unlock(&data->access_lock);

	return err;
};
