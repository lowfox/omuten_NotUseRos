///
/// @author yudai yamaguchi
///
#pragma once
#include <Arduino.h>

template<class CommandType>
class CommandSender {
private:

  uint8_t END_BYTE = 0x00;
  
  Stream* m_stream;

public:

  CommandSender(Stream* stream): m_stream(stream) {}

  bool sendCommand(CommandType command) {
    m_stream->write(2);
    m_stream->write(static_cast<uint8_t>(command));
    m_stream->write(END_BYTE);

    return true;
  }


  bool sendCommand(CommandType command, uint8_t* ptr, size_t size) {
    m_stream->write(size + 2);
    m_stream->write(static_cast<uint8_t>(command));
    m_stream->write(ptr, size);
    m_stream->write(END_BYTE);

    return true;
  }
};
