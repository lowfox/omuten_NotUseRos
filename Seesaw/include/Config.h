///
/// @author yudai yamaguchi
///
#pragma once
#include <stdint.h>

//#define SEESAW_X//青シーソー
#define SEESAW_Y//赤シーソー



/// @brief デバイス識別ID
#if defined SEESAW_X
constexpr uint8_t DEVICE_ID = 0x02;
#elif defined SEESAW_Y
constexpr uint8_t DEVICE_ID = 0x01;
#else
#error("")
#endif


///
/// @brief 受信コマンド
///
enum class ReceiveCommand : uint8_t {
  WHO_AM_I,
  WAKE_UP,
  SLEEP,
  GET_ANGLE,
  BUTTON_LED,

  COMMAND_MAX
};

///
/// @brief 送信コマンド
///
enum class SendCommand : uint8_t {
  ANGLE,
  BUTTON_PUSH,
  RESPONSE,
  
  COMMAND_MAX
};