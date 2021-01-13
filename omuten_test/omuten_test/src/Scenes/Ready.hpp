#pragma once
#include "../App.hpp"

class ReadyScene : public App::Scene
{
private:

    Font font = Font(50);

    double time = 60.0f;
    double prevTime = Scene::Time();

    std::shared_ptr<Seesaw> seesawX;
    std::shared_ptr<Seesaw> seesawY;
    std::shared_ptr<Table> table;

    Audio countStart = Audio(U"Asset/Voice/ready.mp3");
    Audio readyBGM= Audio(U"Asset/Bgm/bgm_ready.mp3");
public:

    ReadyScene(const InitData& init)
        : IScene(init)
    {
        auto& sharedData = getData();

        seesawX = sharedData.seesawX;
        seesawY = sharedData.seesawY;
        table   = sharedData.table;
        countStart.play();
        readyBGM.setVolume(0.1);
        table->boalEmission_init();
    }

    void update() override
    {
        readyBGM.play();
        if (SimpleGUI::Button(U"もどる", Vec2(0, 0)))
        {
            readyBGM.stop();
            changeScene(U"Title", 300, true);
        }

        if (!countStart.isPlaying()) {
            readyBGM.stop();
            changeScene(U"Game");
        }
        // 台サーボを回転
        table->setServoAngle(SeesawToServoAngle(INIT_POS_X, INIT_POS_Y));
        table->turnOnDecorationLED(Palette::Deepskyblue, Palette::Orangered, Float2(seesawX->getAngle(), seesawY->getAngle()));

        table->turnOnTimeLED(Palette::Blue);
        //ボーナスLEDを点灯
        table->bonusLED(1);
        //ボタンLEDを点灯
        seesawX->buttonLED(3);
        seesawY->buttonLED(3);

    }

    void draw() const override
    {
        font(U"もうすぐはじまります").drawAt(Scene::Center());
    }
};