#pragma once
#include "../App.hpp"

class StandbyScene : public App::Scene
{
private:

    Font font = Font(150);
    double time = 60.0f;
    double prevTime = Scene::Time();

    bool readyX = false;
    bool readyY = false;

    std::shared_ptr<Seesaw> seesawX;
    std::shared_ptr<Seesaw> seesawY;
    std::shared_ptr<Table> table;

    Audio buttonXvoice = Audio(U"Asset/Voice/button_1.mp3");
    Audio buttonYvoice = Audio(U"Asset/Voice/button_2.mp3");
    Audio pushButton= Audio(U"Asset/Voice/pushbutton.mp3",Arg::loop = true);
    Audio standbyBGM= Audio(U"Asset/Bgm/bgm_standby.mp3");
    Audio buttonPress= Audio(U"Asset/Se/ctrlButton.mp3");

public:

    StandbyScene(const InitData& init)
        : IScene(init)
    {
        auto& sharedData = getData();

        seesawX = sharedData.seesawX;
        seesawY = sharedData.seesawY;
        table = sharedData.table;
        seesawX->init();
        seesawY->init();
        standbyBGM.setVolume(0.3);
        pushButton.play();

        //DEBUG
        table->boalEmission();
        //
        table->bonusLED(0);
    }

    void update() override
    {
         table->setServoAngle(SeesawToServoAngle(INIT_POS_X, INIT_POS_Y));
        
        if (SimpleGUI::Button(U"もどる", Vec2(0, 0)))
        {
            changeScene(U"Title", 300, true);
        }
        standbyBGM.play();
        if (seesawX->isPushed()) {
            buttonYvoice.stop();
            buttonPress.stop();
            buttonPress.play();
            buttonXvoice.play();
            readyX = true;
        }

        if (seesawY->isPushed()) {
            buttonXvoice.stop();
            buttonPress.stop();
            buttonPress.play();
            buttonYvoice.play();
            readyY = true;
        }
        if (readyX == false && readyY == false) {
            //ゆっくり点滅
            seesawX->buttonLED(1);
            seesawY->buttonLED(1);
 
        }
        else {
            if (readyX == true) {
                seesawX->buttonLED(3);
                if (readyY == false) {
                    seesawY->buttonLED(1);
                }
                else {
                    pushButton.stop();
                }
            }
            if (readyY == true) {
                seesawY->buttonLED(3);
                 if (readyX == false) {
                    seesawX->buttonLED(1);
                    //点滅
                //    seesawX->buttonLED(2);
                 }
                 else {
                    pushButton.stop();
                 }
            }
        }

        // どちらも準備完了
        if (readyX && readyY && !buttonXvoice.isPlaying() && !buttonYvoice.isPlaying()) {
        //if (readyX && !buttonXvoice.isPlaying()) {
            pushButton.stop();
            standbyBGM.stop();
            changeScene(U"Ready");
        }
        table->bonusLED(1);
   }

    void draw() const override
    {
        font(U"ボタンを押してね").drawAt(Scene::Center());
    }
};