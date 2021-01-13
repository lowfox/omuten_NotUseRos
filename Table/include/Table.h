///
/// @author yudai yamaguchi
///
#pragma once
#include "Config.h"
#include <Arduino.h>
#include <Servo.h>
#include "CommandSender.h"
#include <Adafruit_NeoPixel.h>

namespace tapeled
{
Adafruit_NeoPixel gametime_led(m_time_led_cnt, m_time_led_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel x_led(m_xy_led_cnt, m_x_led_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel y_led(m_xy_led_cnt, m_y_led_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel b1_led(m_bonus_led_cnt, m_b1_led_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel b2_led(m_bonus_led_cnt, m_b2_led_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel b3_led(m_bonus_led_cnt, m_b3_led_pin, NEO_GRB + NEO_KHZ800);
} // namespace name

struct Color {
    uint32_t r : 8;
    uint32_t g : 8;
    uint32_t b : 8;
    uint32_t a : 8;
};

class Table {
public:

    static constexpr float ServoCenter = 22.436958 + 90;
    	
private:

    struct ServoInfo {
        Servo servo;
        uint8_t pin;
    };

    ServoInfo servos[4];
    ServoInfo goalServo;
    bool m_isActive = false;

    CommandSender<SendCommand>* m_sender;
    
    bool bonusState=LOW;
    unsigned long pretime=0;
    unsigned long curtime=0;

    unsigned long pretimeTblLED=0;
    unsigned long curtimeTblLED=0;
    bool blinkStateTblLED=LOW;
    uint8_t blinkTblLEDFlg=0;
    uint8_t r_Tbl=0;
    uint8_t g_Tbl=0;
    uint8_t b_Tbl=0;

    unsigned long pretimeBnsLED=0;
    unsigned long curtimeBnsLED=0;
    bool blinkStateBnsLED=LOW;
    bool blinkBnsLEDFlg=LOW;
    uint8_t r_Bns=0;
    uint8_t g_Bns=0;
    uint8_t b_Bns=0;


    float* m_angle;
    float m_angle_x0;
    float m_angle_x1;
    float m_angle_y0;
     float m_angle_y1;


    int m_goalAngle;
public: 

    Table(const uint8_t* pins, CommandSender<SendCommand>* sender) {

        auto pin = pins;

        for(auto& itr : servos){
            itr.pin = *pin;
            pin++;
        }
        goalServo.pin = m_goalServo_pin;
        m_sender = sender;

    }

    bool isActive() {
        return m_isActive;
    }

    ///
    /// @brief 初期化処理
    ///
    void init() {
        // pwm attach
        for(auto& itr : servos){
            itr.servo.attach(itr.pin);
        }
        goalServo.servo.attach(goalServo.pin);
        // center
        rotate(Table::ServoCenter, Table::ServoCenter, Table::ServoCenter, Table::ServoCenter);

        // TO-DO
        // ボール排出用サーボ,ゴール検知、ボーナスエリアの初期化処理
        pinMode(m_goal_pin,INPUT);
        pinMode(m_bonus1_pin,INPUT);
        pinMode(m_bonus2_pin,INPUT);
        pinMode(m_bonus3_pin,INPUT);
        pinMode(m_debug_pin, OUTPUT);
        digitalWrite(m_debug_pin,LOW);
        goalServo.servo.write(63);
        m_isActive = true;
    }

    ///
    /// @brief 終了処理
    ///
    void finalize() {
        if(!isActive()) {
            return;
        }

        for(auto& itr : servos){
            itr.servo.detach();
        }

        // TO-DO
        // ボール排出用サーボ,ゴール検知、ボーナスエリアの終了処理

        goalServo.servo.detach();
        m_isActive = false;
    }

    ///
    /// @brief 台を回転させる
    ///
    void rotate(float x1, float x2, float y1, float y2) {
        m_angle_x0=x1;
        m_angle_x1=x2;
        m_angle_y0=y1;
        m_angle_y1=y2;

    }

    ///
    /// @brief 台を回転させる
    ///
    void rotate(float* angles) {
        if(!isActive()) {
            return;
        }
        
        auto angle = angles;

        for(auto& itr : servos) {
            itr.servo.write(*angle);
            angle++;
        }
        /*
            m_angle_x0=*angles;
            angles++;
            m_angle_x1=*angles;
            angles++;
            m_angle_y0=*angles;
            angles++;
            m_angle_y1=*angles;
            */
    }

    ///
    /// @brief ボールを排出する
    ///
    void boal_emission() {
//        goalServo.servo.write(138);
        m_goalAngle=1840;
    }

    ///
    /// @brief ボール排出初期か処理
    ///
    void boal_emission_init() {
 //       digitalWrite(m_debug_pin,HIGH);
//        goalServo.servo.write(64);
        m_goalAngle=1059;//１度あたり10.5555usec最後に₊５００usec
    }
    ///
    /// @brief 時間表示LEDを点灯する
    ///
    void turnOnTimeLED(const Color& color) {
        r_Tbl = color.r;
        g_Tbl = color.g;
        b_Tbl = color.b;
        if(color.a==253){
            blinkTblLEDFlg=1;
        }else if(color.a==252){
            blinkTblLEDFlg=2;

        }else{
            for(int i=0;i<m_time_led_cnt;i++){
                tapeled::gametime_led.setPixelColor(i,color.r,color.g,color.b);
            }
            tapeled::gametime_led.show();
        }
    }

    ///
    /// @brief 装飾LED(x,y)を点灯する
    ///
    void turnOnDecorationLED(const Color& xColor, const Color& yColor, float x, float y) {
        // TO-DO
        // LED点灯処理
        float val_x = (x*(-1)) + 9.4f;
        float val_y = y + 10.38f;
        val_x /=1.66f;
        val_y /=1.66f;
        int index_x=(int)val_x;
        int index_y=(int)val_y;
        for(int i=0;i<m_xy_led_cnt;i++){
            tapeled::x_led.setPixelColor(i,255,255,255);
            tapeled::y_led.setPixelColor(i,255,255,255);
        }
        tapeled::x_led.setPixelColor(index_x,xColor.r,xColor.g,xColor.b);
        tapeled::y_led.setPixelColor(index_y,yColor.r,yColor.g,yColor.b);
        if(0<=index_x-1){
            tapeled::x_led.setPixelColor(index_x-1,xColor.r,xColor.g,xColor.b);
        }
        if(0<=index_y-1){
            tapeled::y_led.setPixelColor(index_y-1,yColor.r,yColor.g,yColor.b);
        }
        if(m_xy_led_cnt>=index_x+1){
            tapeled::x_led.setPixelColor(index_x+1,xColor.r,xColor.g,xColor.b);
        }
        if(m_xy_led_cnt>=index_y+1){
            tapeled::y_led.setPixelColor(index_y+1,yColor.r,yColor.g,yColor.b);
        }

        tapeled::x_led.show();
        tapeled::y_led.show();
    }

    ///
    /// @brief ボーナスエリアLEDを点灯する
    ///
    void bonusLED(uint8_t type){
        // TO-DO
        //ボーナスエリアのLEDを付ける
        switch(type){
            case 0://虹色:standby,gameover,clear
                for(int i=0; i<m_bonus_led_cnt;i++){
                    tapeled::b1_led.setPixelColor(i,255,255,255);
                    tapeled::b2_led.setPixelColor(i,255,255,255);
                    tapeled::b3_led.setPixelColor(i,255,255,255);
                }
                tapeled::b1_led.show();
                tapeled::b2_led.show();
                tapeled::b3_led.show();
                break;
            case 1://緑:game
                for(int i=0; i<m_bonus_led_cnt;i++){
                    tapeled::b1_led.setPixelColor(i,0,207+(i*4),i*3);
                    tapeled::b2_led.setPixelColor(i,0,207+(i*4),i*3);
                    tapeled::b3_led.setPixelColor(i,0,207+(i*4),i*3);
                }
                tapeled::b1_led.show();
                tapeled::b2_led.show();
                tapeled::b3_led.show();
                break;
            case 2://消灯
                for(int i=0; i<m_bonus_led_cnt;i++){
                    tapeled::b1_led.setPixelColor(i,0,0,0);
                    tapeled::b2_led.setPixelColor(i,0,0,0);
                    tapeled::b3_led.setPixelColor(i,0,0,0);
                }
                tapeled::b1_led.show();
                tapeled::b2_led.show();
                tapeled::b3_led.show();
                break;
            case 3://点滅開始
                blinkBnsLEDFlg=HIGH;

                break;
            case 4://点滅終了
                blinkBnsLEDFlg=LOW;
                break;
            case 5://gameover
                for(int i=0; i<m_bonus_led_cnt;i++){
                    tapeled::b1_led.setPixelColor(i,207+(i*4),i*3,0);
                    tapeled::b2_led.setPixelColor(i,207+(i*4),i*3,0);
                    tapeled::b3_led.setPixelColor(i,207+(i*4),i*3,0);
                }
                tapeled::b1_led.show();
                tapeled::b2_led.show();
                tapeled::b3_led.show();
                break;

        }

    }
 

    ///
    /// @brief 更新処理
    ///
    void update() {

        //DEBUG::サーボ動かす
        /*
        servos[0].servo.write(m_angle_x0);
        servos[1].servo.write(m_angle_x1);
        servos[2].servo.write(m_angle_y0);
        servos[3].servo.write(m_angle_x1);
        */

        goalServo.servo.writeMicroseconds(m_goalAngle);//１度あたり10.5555usec最後に₊５００usec
        //

        // ゴール検知
        if(digitalRead(m_goal_pin)==LOW) {
            m_sender->sendCommand(SendCommand::GOAL);    
            digitalWrite(m_debug_pin,HIGH);
        }
        if(blinkTblLEDFlg==1){
            curtimeTblLED=millis();
            if(curtimeTblLED-pretimeTblLED>300){
                if(blinkStateTblLED==LOW){
                    for(int i=0;i<m_time_led_cnt;i++){
                        tapeled::gametime_led.setPixelColor(i,r_Tbl,g_Tbl,b_Tbl);
                    }
                    tapeled::gametime_led.show();
                    blinkStateTblLED=HIGH;
                }
                else if(blinkStateTblLED==HIGH){
                    for(int i=0;i<m_time_led_cnt;i++){
                        tapeled::gametime_led.setPixelColor(i,0,0,0);
                    }
                    tapeled::gametime_led.show();
                    blinkStateTblLED=LOW;
                }
                pretimeTblLED=curtimeTblLED;
            }
        }else if (blinkTblLEDFlg==2)
        {
            for(int i=0;i<m_time_led_cnt;i++){
                tapeled::gametime_led.setPixelColor(i,r_Tbl,g_Tbl,b_Tbl);
            }
            tapeled::gametime_led.show();
            blinkTblLEDFlg=0;
        }else{
        }

        if(blinkBnsLEDFlg==HIGH){
            curtimeBnsLED=millis();
            if(curtimeBnsLED-pretimeBnsLED>100){
                if(blinkStateBnsLED==LOW){
                    for(int i=0;i<m_bonus_led_cnt;i++){
                        tapeled::b1_led.setPixelColor(i,0,207+(i*4),i*3);
                        tapeled::b2_led.setPixelColor(i,0,207+(i*4),i*3);
                        tapeled::b3_led.setPixelColor(i,0,207+(i*4),i*3);
                    }
                    tapeled::b1_led.show();
                    tapeled::b2_led.show();
                    tapeled::b3_led.show();

                    blinkStateBnsLED=HIGH;
                }
                else if(blinkStateBnsLED==HIGH){
                    for(int i=0;i<m_bonus_led_cnt;i++){
                        tapeled::gametime_led.setPixelColor(i,0,0,0);
                        tapeled::b1_led.setPixelColor(i,0,0,0);
                        tapeled::b2_led.setPixelColor(i,0,0,0);
                        tapeled::b3_led.setPixelColor(i,0,0,0);
                    }
                    tapeled::b1_led.show();
                    tapeled::b2_led.show();
                    tapeled::b3_led.show();
                    blinkStateBnsLED=LOW;
                }
                pretimeBnsLED=curtimeBnsLED;
            }
            
        }

        // ボーナスエリア検知
        if(bonusState==LOW && (digitalRead(m_bonus1_pin)==LOW || digitalRead(m_bonus2_pin)==LOW || digitalRead(m_bonus3_pin)==LOW)) {
            m_sender->sendCommand(SendCommand::BONUS);    
            //TODO:信号を送った後millisで検知しない時間を作る。100msくらい
            bonusState=HIGH;
            pretime=millis();            
        }
        if(bonusState==HIGH){
            curtime=millis();
            if(curtime-pretime>700){
                bonusState=LOW;
            }
        }
    }
};