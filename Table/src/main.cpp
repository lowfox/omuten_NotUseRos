///
/// @author yudai yamaguchi
///
#include "Config.h"
#include <Arduino.h>
#include <CommandReceiver.h>
#include <CommandSender.h>
#include <Table.h>
#include <string.h>


CommandReceiver<ReceiveCommand> receiver(&Serial);
CommandSender<SendCommand> sender(&Serial);
Table table(SERVO_PINS, &sender);
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

  table.init();
  isActive = true;
}

///
/// @brief 寝ろ
///
void sleep_(uint8_t* ptr, uint8_t size) {
  table.finalize();
  isActive = false;
}

///
/// @brief 台
///
void table_rotate(uint8_t* ptr, uint8_t size) {
  if(!isActive) {
    return;
  }

  if(size != 16) {
    return;
  }

  float angles[4];

  ::memcpy(&angles[0], ptr, 16);


  table.rotate(angles);
}

///
/// @brief ボール排出
///
void boal_emission(uint8_t* ptr, uint8_t size) {
  table.boal_emission();
}
///
/// @brief ボール排出初期か処理
///
void boal_emission_init(uint8_t* ptr, uint8_t size) {
  table.boal_emission_init();
}

///
/// @brief 時間表示LED
///
void time_led(uint8_t* ptr, uint8_t size) {
  Color color;

  ::memcpy(&color, ptr, sizeof(Color));

  table.turnOnTimeLED(color);
}

///
/// @brief 装飾LED
///
void decoration_led(uint8_t* ptr, uint8_t size) {
  Color xColor;
  Color yColor;
  float x;
  float y;

  ::memcpy(&xColor, ptr,  sizeof(Color));
  ::memcpy(&yColor, ptr + sizeof(Color), sizeof(Color));
  ::memcpy(&x, ptr+sizeof(Color)*2,  sizeof(float));
  ::memcpy(&y, ptr+sizeof(Color)*2 + sizeof(float), sizeof(float));

  table.turnOnDecorationLED(xColor, yColor, x, y);
}

///
/// @brief 装飾LED
///
void bonus_led(uint8_t* ptr, uint8_t size){
  table.bonusLED(*ptr);
}

///
/// @brief setup
///
void setup() {
  // Uart Init
  Serial.begin(9600);
 // Serial.println("I am Table");
  // CallBacks Register
  receiver.setCallBack(ReceiveCommand::WHO_AM_I,      who_am_i);
  receiver.setCallBack(ReceiveCommand::WAKE_UP,       wake_up);
  receiver.setCallBack(ReceiveCommand::SLEEP,         sleep_);
  receiver.setCallBack(ReceiveCommand::TABLE_ROTATE,  table_rotate);
  receiver.setCallBack(ReceiveCommand::BOAL_EMISSION, boal_emission);
  receiver.setCallBack(ReceiveCommand::BOAL_EMISSION_INIT, boal_emission_init);
  receiver.setCallBack(ReceiveCommand::TIME_LED,      time_led);
  receiver.setCallBack(ReceiveCommand::DECORATION_LED,decoration_led);
  receiver.setCallBack(ReceiveCommand::BONUS_LED,bonus_led);

  tapeled::gametime_led.begin();
  tapeled::gametime_led.show();
  tapeled::x_led.begin();
  tapeled::x_led.show();
  tapeled::y_led.begin();
  tapeled::y_led.show();
  tapeled::b1_led.begin();
  tapeled::b1_led.show();
  tapeled::b2_led.begin();
  tapeled::b2_led.show();
  tapeled::b3_led.begin();
  tapeled::b3_led.show();
  pinMode(2,OUTPUT);
  wake_up(NULL,9);
}

unsigned long preTime = 0;
bool state=false;
///
/// @brief loop
///
void loop() {
///7.8us
  digitalWrite(2,HIGH);
  //receiver.update();
  digitalWrite(2,LOW);

  if(millis()-preTime > 2000){
    if(state == HIGH){
      boal_emission_init(NULL, 9);
      state=LOW;
    }else if (state==LOW){
      boal_emission(NULL, 9);
      state=HIGH;
    }
    preTime=millis();
  }
//4.9us
  if(isActive) {
    table.update();
    /*
    table.updateの時間
    standby 32us
    ready 32us
    game 32us->bonus 54us
    */

 }
//
///
  delay(1);
}



