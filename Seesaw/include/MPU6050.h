///
/// @author yudai yamaguchi
///
#pragma once
#include <stdint.h>
#include <Wire.h>

#define MPU6050_INDEX_AX   0
#define MPU6050_INDEX_AY   1
#define MPU6050_INDEX_AZ   2
#define MPU6050_INDEX_TEMP 3
#define MPU6050_INDEX_GX   4
#define MPU6050_INDEX_GY   5
#define MPU6050_INDEX_GZ   6
#define MPU6050_DATA_NUM   7


class MPU6050 {
public:

    using SensorDataArray = int16_t;

private:

    TwoWire* m_i2c = nullptr;

    uint8_t m_slaveAddr = 0;

    SensorDataArray m_dataArray[MPU6050_DATA_NUM];

public:

    MPU6050() = delete;

    MPU6050(TwoWire* i2c, uint8_t slaveAddr);

    void init();

    void update();

    const SensorDataArray* getSensorData() const {
        return m_dataArray;
    }
};
