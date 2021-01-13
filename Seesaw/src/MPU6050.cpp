///
/// @author yudai yamaguchi
///
#include <MPU6050.h>
#include <Wire.h>

#define MPU6050_AX  0x3B
#define MPU6050_AY  0x3D
#define MPU6050_AZ  0x3F
#define MPU6050_TP  0x41
#define MPU6050_GX  0x43
#define MPU6050_GY  0x45
#define MPU6050_GZ  0x47

MPU6050::MPU6050(TwoWire* i2c, uint8_t slaveAddr):m_i2c(i2c),m_slaveAddr(slaveAddr) {
    for(auto& itr: m_dataArray) {
        itr = 0;
    }
}

void MPU6050::init() {
    // MPU6050 wake up
    m_i2c->beginTransmission(m_slaveAddr);
    m_i2c->write(0x6B);
    m_i2c->write(0);
    m_i2c->endTransmission();
}

void MPU6050::update() {
    // request all data
    m_i2c->beginTransmission(m_slaveAddr);
    m_i2c->write(MPU6050_AX);
    m_i2c->endTransmission();  
    m_i2c->requestFrom(m_slaveAddr, static_cast<uint8_t>(14));

    // sensor data read
    for(auto& itr : m_dataArray) {
        itr  = Wire.read() << 8;
        itr |= Wire.read();
    }
}
