#ifndef ADS7830_SOIL_HUMID_SYSFS
#define ADS7830_SOIL_HUMID_SYSFS

#include "common.h"

int ads7830_soil_humid_init_sysfs(struct ads7830_soil_humid_data *data);

void ads7830_soil_humid_destroy_sysfs(struct ads7830_soil_humid_data *data);

#endif
