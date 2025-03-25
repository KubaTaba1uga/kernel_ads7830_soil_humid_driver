#include <linux/i2c.h>

#include "common.h"

int ads7830_soil_humid_init(struct i2c_client *client)
{
	struct ads7830_soil_humid_data *data;

	// Memory allocated with this function is automatically freed on driver
	//   detach.
	data = devm_kzalloc(&client->dev, sizeof(struct ads7830_soil_humid_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;

	// This functions sets i2c client data so it can be acessed from anwyehre in
	// this i2c device ctx.
	i2c_set_clientdata(client, data);

	return 0;
}
