#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "FreeRTOS.h"
#include "device.h"
#include "rtl8195a/rtl_libc.h"
#include "platform_stdlib.h"

#include "timer_api.h"
#include "driver/i2c_drv.h"
#include "pinmap.h"

#define MPU6050_IRQTIMER		TIMER0
#define MPU6050_I2C_ADDR		0x68
#define MPU6050_PACKSIZE		4096
#define MPU6050_PERIOD_US		1000

typedef struct _MPU6050_Data
{
	// Приватные   
	i2c_drv_t i2c;
	gtimer_t timer;

	// Публичные
	int16_t Accel_X;
	int16_t Accel_Y;
	int16_t Accel_Z;
	int16_t Gyro_X;
	int16_t Gyro_Y;
	int16_t Gyro_Z;
	float   Temp_Deg;
} MPU6050_Data, *MPU6050_Data_p;

typedef struct
{
	double X;
	//int16_t Y;
	//int16_t Z;
} MPU6050_DataPack;

typedef enum
{
	MPU6050_Accelerometer_2G = 0x00,
	MPU6050_Accelerometer_4G = 0x01,
	MPU6050_Accelerometer_8G = 0x02,
	MPU6050_Accelerometer_16G = 0x03
} MPU6050_Accelerometer;

typedef enum
{
	MPU6050_Bandwidth_Infinity = 0x00,
	MPU6050_Bandwidth_184Hz = 0x01,
	MPU6050_Bandwidth_94Hz = 0x02,
	MPU6050_Bandwidth_44Hz = 0x03,
	MPU6050_Bandwidth_21Hz = 0x04,
	MPU6050_Bandwidth_10Hz = 0x05,
	MPU6050_Bandwidth_5Hz = 0x06,
} MPU6050_DLFP;

typedef enum
{
	MPU6050_Gyroscope_250s = 0x00,
	MPU6050_Gyroscope_500s = 0x01,
	MPU6050_Gyroscope_1000s = 0x02,
	MPU6050_Gyroscope_2000s = 0x03
} MPU6050_Gyroscope;

typedef enum
{
	MPU6050_RESULT_OK = 1,
	MPU6050_RESULT_NO = 0,
	MPU6050_RESULT_ERROR = -1,
	MPU6050_RESULT_DEVNOTCONN = -2
} MPU6050_Result;

#define MPU6050_DataRate_8KHz       0
#define MPU6050_DataRate_4KHz       1
#define MPU6050_DataRate_2KHz       3
#define MPU6050_DataRate_1KHz       7
#define MPU6050_DataRate_500Hz      15
#define MPU6050_DataRate_250Hz      31
#define MPU6050_DataRate_125Hz      63 
#define MPU6050_DataRate_100Hz      79

#define MPU6050_AUX_VDDIO					0x01
#define MPU6050_SMPLRT_DIV					0x19
#define MPU6050_CONFIG						0x1A
#define MPU6050_GYRO_CONFIG					0x1B
#define MPU6050_ACCEL_CONFIG				0x1C
#define MPU6050_MOTION_THRESH				0x1F
#define MPU6050_INT_PIN_CFG					0x37
#define MPU6050_INT_ENABLE					0x38
#define MPU6050_INT_STATUS					0x3A
#define MPU6050_ACCEL_XOUT_H				0x3B
#define MPU6050_ACCEL_XOUT_L				0x3C
#define MPU6050_ACCEL_YOUT_H				0x3D
#define MPU6050_ACCEL_YOUT_L				0x3E
#define MPU6050_ACCEL_ZOUT_H				0x3F
#define MPU6050_ACCEL_ZOUT_L				0x40
#define MPU6050_TEMP_OUT_H					0x41
#define MPU6050_TEMP_OUT_L					0x42
#define MPU6050_GYRO_XOUT_H					0x43
#define MPU6050_GYRO_XOUT_L					0x44
#define MPU6050_GYRO_YOUT_H					0x45
#define MPU6050_GYRO_YOUT_L					0x46
#define MPU6050_GYRO_ZOUT_H					0x47
#define MPU6050_GYRO_ZOUT_L					0x48
#define MPU6050_MOT_DETECT_STATUS			0x61
#define MPU6050_SIGNAL_PATH_RESET			0x68
#define MPU6050_MOT_DETECT_CTRL				0x69
#define MPU6050_USER_CTRL					0x6A
#define MPU6050_PWR_MGMT_1					0x6B
#define MPU6050_PWR_MGMT_2					0x6C
#define MPU6050_FIFO_COUNTH					0x72
#define MPU6050_FIFO_COUNTL					0x73
#define MPU6050_FIFO_R_W					0x74
#define MPU6050_WHO_AM_I					0x75
#define MPU6050_I_AM						0x68

// Gyro sensitivities in degrees/s 
#define MPU6050_GYRO_SENS_250				((double) 131.0)
#define MPU6050_GYRO_SENS_500				((double) 65.5)
#define MPU6050_GYRO_SENS_1000				((double) 32.8)
#define MPU6050_GYRO_SENS_2000				((double) 16.4)

// Acce sensitivities in g/s
#define MPU6050_ACCE_SENS_2					((double) 16384.0)
#define MPU6050_ACCE_SENS_4					((double) 8192.0)
#define MPU6050_ACCE_SENS_8					((double) 4096.0)
#define MPU6050_ACCE_SENS_16				((double) 2048.0)

int16_t MPU6050_Init();
void MPU6050_SetGyroscope(MPU6050_Gyroscope gs);
void MPU6050_SetAccelerometer(MPU6050_Accelerometer as);
void MPU6050_SetDataRate(uint8_t rate);
int16_t MPU6050_IsDataPacketReady(MPU6050_DataPack *dp);
void MPU6050_OffsetCalibrate(int32_t num);
void MPU6050_SetDLPF(uint8_t Bandwidth);

#endif // _MPU6050_H_
