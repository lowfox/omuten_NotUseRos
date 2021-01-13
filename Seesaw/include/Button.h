///
/// @author yudai yamaguchi
///
#pragma once
#include <Arduino.h>

class Button {
private:

    uint8_t m_pin;
    uint8_t m_ledPin;
    bool ledState;
    unsigned long curMillis;
    unsigned long preMillis;
    unsigned long intervalnum=700;;
    int m_prevInput = 0;

    bool m_isPushed = false;

public:

    Button(uint8_t pin,uint8_t ledPin) : m_pin(pin), m_ledPin(ledPin){}

    void init() {
        pinMode(m_pin, INPUT_PULLUP);
        pinMode(m_ledPin, OUTPUT);
        m_prevInput = digitalRead(m_pin);
        preMillis=millis();
        ledState=LOW;
        m_isPushed = false;
    }

    void ledOn(){
        digitalWrite(m_ledPin,HIGH);
        
    }

    void ledOff(){
        digitalWrite(m_ledPin,LOW);
    }


    void finalize() {
        m_prevInput = 0;
        m_isPushed = false;
    }

    void update() {
        auto input = digitalRead(m_pin);
        if((!input) && (m_prevInput)) {
            m_isPushed = true;
        } else {
            m_isPushed = false;
        }

        m_prevInput = input;
    }

    bool isPushed() {
        return m_isPushed;
    }
    
    void button_led(uint8_t type){

        switch(type){
            case 0://消灯
                ledOff();
                break;
            case 1://ゆっくり点滅
                curMillis=millis();
                if(curMillis-preMillis<600){

                    analogWrite(m_ledPin,(curMillis-preMillis)/3+55);
                }else{
                    preMillis=curMillis;
                }
                break;
            case 2://点滅(500ms)
                digitalWrite(13,HIGH);
                curMillis = millis();
                if(curMillis-preMillis>intervalnum){
                    if(ledState==LOW){
                        digitalWrite(m_ledPin,HIGH);
                        ledState=HIGH;
                    }else if(ledState==HIGH){
                        digitalWrite(m_ledPin,LOW);
                        ledState=LOW;
                    }else{

                    }
                }
                preMillis=curMillis;
                
                break;
            case 3://点灯
                ledOn();
                break;
        }
    }
};