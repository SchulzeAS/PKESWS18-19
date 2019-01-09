#ifndef IMU_H
#define IMU_H

#include <inttypes.h>

#define IMU_ADDR  0x69

struct IMUdata
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
};

struct IMUdataf
{
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
};

void initializeIMU();
void readIMU(struct IMUdata& d);
uint8_t IMUWhoAreYou();
void readIMUscale(struct IMUdataf& d);
#endif
