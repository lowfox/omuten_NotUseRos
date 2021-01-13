#pragma once
#include <Siv3D.hpp>
#include "../App.hpp"

class GameScene : public App::Scene
{
private:

    Font font = Font(200);
    double prevTime = Scene::Time();
    double time = 60.0f;

    std::shared_ptr<Seesaw> seesawX;
    std::shared_ptr<Seesaw> seesawY;
    std::shared_ptr<Table> table;

   
    unsigned long m_curTimeBlinkBns=0;
    unsigned long m_preTimeBlinkBns=0;
    bool m_stateBlinkBns = false;
    bool m_flgBlinkBns= false;
    int m_cnt = 0;
    int m_blinkCnt=6;

    class TimeEffect {
    public:

        struct TimeAction {
            Audio audio;
            std::function<void()> func;
            double time;
        };

    private:

        double prev;

        Array<TimeAction> m_actions;


    public:

        void init(double time, Array<TimeAction>& actions) {
            prev = time;
            m_actions = actions;
        }

        void update(double time) {
            for (auto& itr : m_actions) {
                if (time < itr.time && prev >= itr.time) {
                    itr.audio.stop();
                    itr.audio.play();
                    itr.func();
                }
                if (time > 11.1f && time <31.0f) {
                    //ボーナスで黄色になった時
                   // m_actions[2].func();
                }  
                if(time > 31.1f) {
                    
                   //ボーナスで青になった時
                    //m_actions[5].func();

                }
            }

            prev = time;
        }
    };

    TimeEffect timeEffect;
    Audio bonusVoice = Audio(U"Asset/Voice/bonus.mp3");
    Audio gameBGM= Audio(U"Asset/Bgm/bgm_game.mp3");
    Audio goalIn= Audio(U"Asset/Se/goalin.mp3");
    Audio bonus= Audio(U"Asset/Se/bonus.mp3");
    Audio buzzer= Audio(U"Asset/Se/buzzer.mp3");
    Audio fivecnt= Audio(U"Asset/Voice/5cnt.mp3");

public:

    GameScene(const InitData& init)
        : IScene(init)
    {
        auto& sharedData = getData();
            
        seesawX = sharedData.seesawX;
        seesawY = sharedData.seesawY;
        table   = sharedData.table;


        Array<TimeEffect::TimeAction> actions{
            {Audio(U"Asset/Voice/10.mp3"),[this] {table->turnOnTimeLED(Palette::Red); },11.0f},
            {Audio(U"Asset/Voice/20.mp3"),[] {},21.0f},
            {Audio(U"Asset/Voice/30.mp3"),[this] {table->turnOnTimeLED(Palette::Yellow); },31.0f},
            {Audio(U"Asset/Se/damage.mp3"),[this] {table->turnOnTimeLED(Palette::Black); },1.0f},
            {Audio(U"Asset/Se/damage.mp3"),[this] {table->turnOnTimeLED(Palette::Yellow); },11.1f},
            {Audio(U"Asset/Se/damage.mp3"),[this] {table->turnOnTimeLED(Palette::Blue); },31.1f},
            {Audio(U"Asset/Voice/1.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,252)); },2.0f},//赤点滅終了
            {Audio(U"Asset/Voice/2.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,253)); },3.0f},
            {Audio(U"Asset/Voice/3.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,253)); },4.0f},
            {Audio(U"Asset/Voice/4.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,253)); },5.0f},
            {Audio(U"Asset/Voice/5.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,253)); },6.0f},//赤点滅開始
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,252)); },7.0f},//赤点滅しない
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,252)); },8.0f},//赤点滅しない
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,252)); },9.0f},//赤点滅しない
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,0,0,252)); },10.0f},//赤点滅しない 
            {Audio(U"Asset/Voice/6.mp3"),[] {},7.0f},
            {Audio(U"Asset/Voice/7.mp3"),[] {},8.0f},
            {Audio(U"Asset/Voice/8.mp3"),[] {},9.0f},
            {Audio(U"Asset/Voice/9.mp3"),[] {},10.0f},
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(0,0,255,252)); },41.0f},//青点滅しない
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(0,0,255,252)); },40.0f},//青点滅しない
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(0,0,255,252)); },39.0f},//青点滅しない
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(0,0,255,252)); },38.0f},//青点滅しない
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(0,0,255,252)); },37.0f},//青点滅しない
            {Audio(U""),[this] {table->turnOnTimeLED(Color(0,0,255,253)); },36.0f},//青点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(0,0,255,253)); },35.0f},//青点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(0,0,255,253)); },34.0f},//青点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(0,0,255,253)); },33.0f},//青点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(0,0,255,252)); },32.0f},//青点滅終了
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,255,0,252)); },21.0f},//黄点滅しない 
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,255,0,252)); },20.0f},//黄点滅しない 
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,255,0,252)); },19.0f},//黄点滅しない 
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,255,0,252)); },18.0f},//黄点滅しない 
            {Audio(U"Asset/Se/muon.mp3"),[this] {table->turnOnTimeLED(Color(255,255,0,252)); },17.0f},//黄点滅しない 
            {Audio(U""),[this] {table->turnOnTimeLED(Color(255,255,0,253)); },16.0f},//黄点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(255,255,0,253)); },15.0f},//黄点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(255,255,0,253)); },14.0f},//黄点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(255,255,0,253)); },13.0f},//黄点滅開始
            {Audio(U""),[this] {table->turnOnTimeLED(Color(255,255,0,252)); },12.0f},//黄点滅終了

        };

        m_cnt = 0;
        table->init();  
        table->turnOnTimeLED(Palette::Blue);
        //DEBUG
        table->boalEmission_init();
        //
        gameBGM.setVolume(1.0);
        goalIn.setVolume(0.8);
        bonus.setVolume(0.8);
        buzzer.setVolume(0.8);
        //ボタンLEDを点灯
        seesawX->buttonLED(3);
        seesawY->buttonLED(3);
        fivecnt.play();
        while (fivecnt.isPlaying()) {
        }
        buzzer.play();
        gameBGM.play();
        timeEffect.init(Scene::Time()-prevTime, actions);
    }

    void update() override
    {

            auto now = Scene::Time();
            time -= (now - prevTime);
            prevTime = now;

            timeEffect.update(time);
            if (SimpleGUI::Button(U"もどる", Vec2(0, 0)))
            {
                gameBGM.stop();
                changeScene(U"Title", 300, true);
            }

            if (time <= 0.0f) {
                buzzer.play();

                //ボタンLEDを消灯
                seesawX->buttonLED(0);
                seesawY->buttonLED(0);
                gameBGM.stop();
                changeScene(U"GameOver", 300, true);
            }

            // ゴール
            if (table->isGoal()) {
                goalIn.play();

                //ボタンLEDを消灯
                seesawX->buttonLED(0);
                seesawY->buttonLED(0);
                gameBGM.stop();
                changeScene(U"GameClear");
            }

            // ボーナス
            if (table->isBonus()) {
                time += 5;
                //ボーナスSE
                bonus.play();
                //ボーナスLEDを点滅
                m_flgBlinkBns = true;
                bonusVoice.playOneShot();
            }
            if (m_flgBlinkBns == true) {
                static int i = 0;
                if (i == 0) {
                    m_preTimeBlinkBns = Scene::Time();
                    i++;
                }
                    //ボーナスLEDを点滅
                    m_curTimeBlinkBns = Scene::Time();
                    if (m_curTimeBlinkBns - m_preTimeBlinkBns > 1.5f) {
                        table->bonusLED(4);//点滅終了
                        table->bonusLED(1);
                        m_flgBlinkBns = false;
                        i = 0;

                    }else {
                        table->bonusLED(3);//点滅開始


                    }
            }


            // 台サーボを回転
            table->setServoAngle(SeesawToServoAngle(seesawX->getAngle(), seesawY->getAngle()));
            // Print << "time:" << Scene::Time();
            table->turnOnDecorationLED(Palette::Deepskyblue, Palette::Orangered, Float2(seesawX->getAngle(), seesawY->getAngle()));

    }

    void draw() const override
    {
        font(Format(static_cast<int32>(time))).drawAt(Scene::Center());
    }
};