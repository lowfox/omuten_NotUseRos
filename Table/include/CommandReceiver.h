///
/// @author yudai yamaguchi
///
#pragma once
#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

///
/// @brief コマンドレシーバー
///
template<class CommandType, uint8_t CommandMAX = static_cast<uint8_t>(CommandType::COMMAND_MAX)>
class CommandReceiver {
public:

  /// @brief コマンド受信時に呼ばれるコールバック関数
  using CallBack = void (*)(uint8_t* ptr, uint8_t size);

private:

  enum class State {
    IDLE,
    RECEIVEING
  };

  Stream* m_stream;

  State m_state = State::IDLE;

  uint8_t packetSize = 0;

  CallBack m_callBackArray[CommandMAX];

  uint8_t m_buffer[64];

public:

  CommandReceiver(Stream* stream): m_stream(stream) {
    ::memset(m_callBackArray, 0, sizeof(CallBack)*CommandMAX);
  }

  /// @brief 更新処理　毎Loop呼ぶこと
  void update() {
    while(true) {
      switch (m_state)
      {
      case State::IDLE:
        if(m_stream->available()) {
          packetSize = m_stream->read();
          m_state = State::RECEIVEING;
        }else {
          return;
        }
        break;
      case State::RECEIVEING:
        if(m_stream->available() >= packetSize) {
          m_stream->readBytes(m_buffer, packetSize);
          if(m_callBackArray[static_cast<uint8_t>(m_buffer[0])] != NULL) {
            m_callBackArray[m_buffer[0]](&m_buffer[1], packetSize - 1);
          }
          m_state = State::IDLE;
        }else {
          return;
        }
        break;
      default:
        abort();
        break;
      }
    }
  }

  ///
  /// @brief コマンド受信時のコールバックを登録する
  ///
  void setCallBack(CommandType command, CallBack callback) {
    m_callBackArray[static_cast<uint8_t>(command)] = callback;
  }
};