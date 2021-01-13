///
/// @author yudai yamaguchi
///
#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <kalman.h>
#include <MPU6050.h>

class Seesaw {
private:

    uint32_t m_time = micros();

    double m_kalmanAngle = 0.0f;

    double m_gyroAngle = 0.0f;

    Kalman m_kalman;

    MPU6050 m_mpu6050;

public:

    Seesaw(TwoWire* i2c) : m_mpu6050(MPU6050(i2c,0x68)) {}

    void init() {
        m_mpu6050.init();

        delay(100);

        m_mpu6050.update();

        auto data  = m_mpu6050.getSensorData();

        //
        // References https://github.com/TKJElectronics/KalmanFilter/tree/master/examples/MPU6050
        //
        double roll = atan2(data[MPU6050_INDEX_AX] , data[MPU6050_INDEX_AZ]) * RAD_TO_DEG;
        m_kalman.setAngle(roll);
        m_gyroAngle = roll;

        m_time = micros();
    }

    void finalize() {

    }

    void update() {
        //
        // References https://github.com/TKJElectronics/KalmanFilter/tree/master/examples/MPU6050
        //
        double dt = (double)(micros() - m_time) / 1000000;
        m_time = micros();

        m_mpu6050.update();

        auto data  = m_mpu6050.getSensorData();
        double roll = atan2(data[MPU6050_INDEX_AX] , data[MPU6050_INDEX_AZ]) * RAD_TO_DEG;
        double gyroRate = data[MPU6050_INDEX_GX] / 131.0; // Convert to deg/s

        if ((roll < -90 && m_kalmanAngle > 90) || (roll > 90 && m_kalmanAngle < -90)) {
            m_kalman.setAngle(roll);
            m_kalmanAngle = roll;
            m_gyroAngle   = roll;
        } else{
            m_kalmanAngle = m_kalman.getAngle(roll, gyroRate, dt); // Calculate the angle using a Kalman filter
        }
    }

    float getAngle() const {
        return static_cast<float>(m_kalmanAngle);
    }

    
};