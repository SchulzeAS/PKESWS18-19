#include <Arduino.h>
#include <Wire.h>
#include "IMU.h"
#include "IMURegisters.h"

#define IMU_Addr     0b1101001
#define I2C_clock    400000UL

#define ACCEL_FS_SEL 00
#define GYRO_FS_SEL  00
#define ph 1337
#define IMUscale 16.384

uint8_t IMU_WhoAmI;

void initializeIMU()
{
    uint8_t ret;
    //I2C Master
    Wire.begin();

    Wire.setClock(I2C_clock);

    //read WhoAmI
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(IMU_WHO_AM_I);
    ret = Wire.endTransmission(false);

    if(ret)
    {
        IMU_WhoAmI=ret;
        return;
    }

    Wire.requestFrom(IMU_ADDR,1);
    IMU_WhoAmI=Wire.read();

    //disable powersave modes
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(IMU_PWR_MGMT_1);
    Wire.write(0);
    //and _PWR_MGMT_2
    Wire.write(0);
    Wire.endTransmission();
//configuration
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(IMU_GYRO_CONFIG);
    Wire.write(0|GYRO_FS_SEL<<3);
//  (IMU_ACCEL_CONFIG);
    Wire.write(0|ACCEL_FS_SEL<<3);
    Wire.endTransmission();
    //selftest ?
}

void readIMU(struct IMUdata& d)
{
    //read accelleromenter, temprature and gyroscope
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(IMU_ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(IMU_ADDR,14);
    d.accel_x = Wire.read() << 8;
    d.accel_x |= Wire.read();
    d.accel_y = Wire.read() << 8;
    d.accel_y |= Wire.read();
    d.accel_z = Wire.read() << 8;
    d.accel_z |= Wire.read();
    //skip Temperature
    Wire.read();
    Wire.read();
//     Wire.beginTransmission(IMU_ADDR);
//     Wire.write(IMU_GYRO_XOUT_H);
//     Wire.endTransmission(false);
//     Wire.requestFrom(IMU_ADDR,6);
    d.gyro_x = Wire.read() << 8;
    d.gyro_x |= Wire.read();
    d.gyro_y = Wire.read() << 8;
    d.gyro_y |= Wire.read();
    d.gyro_z = Wire.read() << 8;
    d.gyro_z |= Wire.read();
}

void readIMUscale(struct IMUdataf& d)
{
struct IMUdata k;
	readIMU(k);
	d.gyro_x = (float) k.gyro_x / 131.0f;
	d.gyro_y = (float) k.gyro_y / 131.0f;
	d.gyro_z = (float) k.gyro_z / 131.0f;
	d.accel_x = (float) k.accel_x /IMUscale;
	d.accel_y = (float) k.accel_y /IMUscale;
	d.accel_z = (float) k.accel_z /IMUscale;
	
	
}

uint8_t IMUWhoAreYou()
{
    return IMU_WhoAmI;
}
