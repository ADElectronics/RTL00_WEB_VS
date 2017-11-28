#include "MPU6050.h"
#include "kalman.h"

MPU6050_DataPack dataPack1[MPU6050_PACKSIZE], dataPack2[MPU6050_PACKSIZE];
uint16_t currentDataPackPointer = 0;
uint8_t currentDataPack = 0;
uint8_t previousDataPackReady = 0;

MPU6050_Data mpu =
{
	.i2c.idx = 3,				// I2C3
	.i2c.io_sel = S0,			// PB_2, PB_3
	.i2c.mode = DRV_I2C_FS_MODE,
};

void MPU6050T_irq_handler(uint32_t *par)
{
	MPU6050_Data_p p = &mpu;

	// MPU6050_ReadGyroscope
	uint8_t data[2];
	data[0] = MPU6050_GYRO_XOUT_H; // MPU6050_ACCEL_XOUT_H;// MPU6050_GYRO_XOUT_H;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &data[0], 1, 0);
	_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &data, 2, 1);

	p->Gyro_X = (int16_t)(data[0] << 8 | data[1]);

	data[0] = MPU6050_ACCEL_XOUT_H;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &data[0], 1, 0);
	_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &data, 2, 1);

	p->Accel_X = (int16_t)(data[0] << 8 | data[1]);

	if (currentDataPack)
	{
		dataPack2[currentDataPackPointer].X = Kalman_GetAngle(p->Accel_X, p->Gyro_X, 0.001);
	}
	else
	{
		dataPack1[currentDataPackPointer].X = Kalman_GetAngle(p->Accel_X, p->Gyro_X, 0.001);
	}

	currentDataPackPointer++;
	if (currentDataPackPointer >= MPU6050_PACKSIZE)
	{
		currentDataPackPointer = 0;
		currentDataPack = !currentDataPack;
		previousDataPackReady = 1;
	}
}

int16_t MPU6050_Init()
{
	MPU6050_Data_p p = &mpu;
	uint8_t temp;
	uint8_t d[2];

	Kalman_Init();

	_i2c_init(&p->i2c);

	d[0] = MPU6050_PWR_MGMT_1;
	d[1] = 0x00;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &d, 2, 1);


	temp = MPU6050_WHO_AM_I;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 0);
	_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 1);

	if (temp != MPU6050_I_AM)
	{
		DiagPrintf("MPU6050. Init Error. ID = %d (default:%d)\n", temp, MPU6050_I_AM);
		return MPU6050_RESULT_ERROR;
	}
	else
	{
		DiagPrintf("MPU6050. Init Done. ID = %d\n", temp);
	}

	MPU6050_SetDataRate(MPU6050_DataRate_8KHz);
	MPU6050_SetAccelerometer(MPU6050_Accelerometer_2G);
	MPU6050_SetGyroscope(MPU6050_Gyroscope_250s);
	MPU6050_SetDLPF(MPU6050_Bandwidth_Infinity);

	MPU6050_OffsetCalibrate(2000);

	gtimer_init(&p->timer, MPU6050_IRQTIMER);
	gtimer_start_periodical(&p->timer, MPU6050_PERIOD_US, (void*)MPU6050T_irq_handler, (uint32_t)&mpu);

	return MPU6050_RESULT_OK;
}

void MPU6050_SetGyroscope(MPU6050_Gyroscope gs)
{
	MPU6050_Data_p p = &mpu;
	uint8_t temp;
	temp = MPU6050_GYRO_CONFIG;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 0);
	_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 1);

	temp = (temp & 0xE7) | (uint8_t)gs << 3;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 1);
}

void MPU6050_SetAccelerometer(MPU6050_Accelerometer as)
{
	MPU6050_Data_p p = &mpu;
	uint8_t temp;
	temp = MPU6050_ACCEL_CONFIG;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 0);
	_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 1);

	temp = (temp & 0xE7) | (uint8_t)as << 3;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &temp, 1, 1);
}

void MPU6050_SetDataRate(uint8_t rate)
{
	MPU6050_Data_p p = &mpu;
	uint8_t d[2];

	d[0] = MPU6050_SMPLRT_DIV;
	d[1] = rate;

	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &d, 2, 1);
}
int16_t MPU6050_IsDataPacketReady(MPU6050_DataPack *dp)
{
	if (previousDataPackReady)
	{
		previousDataPackReady = 0;

		if (currentDataPack)
		{
			memcpy(dp, &dataPack1, sizeof(MPU6050_DataPack) * MPU6050_PACKSIZE);
		}
		else
		{
			memcpy(dp, &dataPack2, sizeof(MPU6050_DataPack) * MPU6050_PACKSIZE);
		}
		return MPU6050_RESULT_OK;
	}

	return MPU6050_RESULT_NO;
}

void MPU6050_OffsetCalibrate(int32_t num)
{
	MPU6050_Data_p p = &mpu;
	int32_t x = 0, y = 0, z = 0, i;
	uint8_t data[6];

	DiagPrintf("MPU6050. Calibrating gyroscope, don't move the hardware!\n");
	//vTaskDelay(500);

	data[0] = MPU6050_GYRO_XOUT_H;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &data[0], 1, 0);
	_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &data, 6, 1);

	x = (int16_t)(data[0] << 8 | data[1]);
	y = (int16_t)(data[2] << 8 | data[3]);
	z = (int16_t)(data[4] << 8 | data[5]);

	for (i = 0; i < num; i++)
	{
		data[0] = MPU6050_GYRO_XOUT_H;
		_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &data[0], 1, 0);
		_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &data, 6, 1);

		x = (x + (int16_t)(data[0] << 8 | data[1])) / 2;
		y = (y + (int16_t)(data[2] << 8 | data[3])) / 2;
		z = (z + (int16_t)(data[4] << 8 | data[5])) / 2;
	}

	//gyro_x_OC = x;
	//gyro_y_OC = y;
	//gyro_z_OC = z;

	DiagPrintf("MPU6050. Gyroscope offsets: X = %d Y = %d Z = %d\n", x, y, z);

	data[0] = MPU6050_ACCEL_XOUT_H;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &data[0], 1, 0);
	_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &data, 6, 1);

	DiagPrintf("MPU6050. Calibrating accelrometer, don't move the hardware!\n");

	x = (int16_t)(data[0] << 8 | data[1]);
	y = (int16_t)(data[2] << 8 | data[3]);
	z = (int16_t)(data[4] << 8 | data[5]);

	for (i = 0; i < num; i++)
	{
		data[0] = MPU6050_ACCEL_XOUT_H;
		_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &data[0], 1, 0);
		_i2c_read(&p->i2c, MPU6050_I2C_ADDR, &data, 6, 1);

		x = (x + (int16_t)(data[0] << 8 | data[1])) / 2;
		y = (y + (int16_t)(data[2] << 8 | data[3])) / 2;
		z = (z + (int16_t)(data[4] << 8 | data[5])) / 2;
	}

	//accel_x_OC = x;
	//accel_y_OC = y;
	//accel_z_OC = z - (float)g * 1000 / accel_scale_fact;

	DiagPrintf("MPU6050. Accelrometer offsets: X = %d Y = %d Z = %d\n", x, y, z);
}

void MPU6050_SetDLPF(uint8_t Bandwidth)
{
	MPU6050_Data_p p = &mpu;
	uint8_t data[2];

	if (Bandwidth < MPU6050_Bandwidth_Infinity || Bandwidth > MPU6050_Bandwidth_5Hz)
	{
		Bandwidth = MPU6050_Bandwidth_Infinity;
	}
	
	data[0] = MPU6050_CONFIG;
	data[1] = Bandwidth;
	_i2c_write(&p->i2c, MPU6050_I2C_ADDR, &data[0], 2, 1);
}
