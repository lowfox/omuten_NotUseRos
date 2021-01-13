///
/// @author yudai yamaguchi
///
#include <Arduino.h>
#include <CommandReceiver.h>
#include <CommandSender.h>
#include <Config.h>
#include <Seesaw.h>
#include <Button.h>

CommandReceiver<ReceiveCommand> receiver(&Serial);
CommandSender<SendCommand> sender(&Serial);
Seesaw seesaw(&Wire);
Button button(14,6);
bool isActive = false;


///
/// @brief お前は誰だ
///
void who_am_i(uint8_t* ptr, uint8_t size) {
  uint8_t id = DEVICE_ID;
  sender.sendCommand(SendCommand::RESPONSE, &id, 1);
}

///
/// @brief 起きろ
///
void wake_up(uint8_t* ptr, uint8_t size) {
  if(isActive) {
    return;
  }

  Wire.begin();
  seesaw.init();
  button.init();
  isActive = true;
}

///
/// @brief 寝ろ
///
void sleep_(uint8_t* ptr, uint8_t size) {
  seesaw.finalize();
  button.finalize();
  Wire.end();
  
  isActive = false;
}

///
/// @brief 角度要求
///
void get_angle(uint8_t* ptr, uint8_t size) {
  float angle = seesaw.getAngle();
  sender.sendCommand(SendCommand::ANGLE, reinterpret_cast<uint8_t*>(&angle), sizeof(angle));
}

///
/// @brief ボタンLED 
///
void button_led(uint8_t* ptr, uint8_t size){
  button.button_led(*ptr);
}

void setup() {
    // Uart Init
  Serial.begin(9600);
  pinMode(13,OUTPUT);

  // CallBacks Register
  receiver.setCallBack(ReceiveCommand::WHO_AM_I,    who_am_i);
  receiver.setCallBack(ReceiveCommand::WAKE_UP,     wake_up);
  receiver.setCallBack(ReceiveCommand::SLEEP,       sleep_);
  receiver.setCallBack(ReceiveCommand::GET_ANGLE,   get_angle);
  receiver.setCallBack(ReceiveCommand::BUTTON_LED,  button_led);
}

void loop() {
  receiver.update();

  if(isActive) {
    seesaw.update();
    button.update();

    if(button.isPushed()) {
      sender.sendCommand(SendCommand::BUTTON_PUSH);
    }
  } else {
    delay(1);
  }

  delay(1);
}