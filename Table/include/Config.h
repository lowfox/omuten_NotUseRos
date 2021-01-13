///
/// @author yudai yamaguchi
///
#pragma once
#include <stdint.h>

/// @brief デバイス識別ID
constexpr uint8_t DEVICE_ID = 0x04;

/// @brief サーボモータのピン配列
constexpr uint8_t SERVO_PINS[4]  = {3,10,5,6};   
constexpr uint8_t m_goalServo_pin=11;

/// @brief センサーのピン配列
constexpr uint8_t m_goal_pin = 14;
constexpr uint8_t m_bonus1_pin = 15;
constexpr uint8_t m_bonus2_pin = 16;
constexpr uint8_t m_bonus3_pin = 17; 

/// @brief デバッグLEDのピン配列
constexpr uint8_t m_debug_pin =13;

/// @brief テープLEDのピン配列
constexpr uint8_t m_time_led_pin=7;
constexpr uint8_t m_b1_led_pin=8;
constexpr uint8_t m_b2_led_pin=9;
constexpr uint8_t m_b3_led_pin=4;
constexpr uint8_t m_x_led_pin=18;
constexpr uint8_t m_y_led_pin=12;

/// @brief テープLED個数
constexpr uint8_t m_time_led_cnt =19;
constexpr uint8_t m_bonus_led_cnt =12;
constexpr uint8_t m_xy_led_cnt =22;

///
/// @brief 受信コマンド
///
enum class ReceiveCommand : uint8_t {
  WHO_AM_I,
  WAKE_UP,
  SLEEP,
  TABLE_ROTATE,
  BOAL_EMISSION,
  BOAL_EMISSION_INIT,
  TIME_LED,
  DECORATION_LED,
  BONUS_LED,

  COMMAND_MAX
};

///
/// @brief 送信コマンド
///
enum class SendCommand : uint8_t {
  GOAL,
  BONUS,
  RESPONSE,
  
  COMMAND_MAX
};