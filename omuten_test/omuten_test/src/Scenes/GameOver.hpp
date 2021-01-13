#pragma once
#include <Siv3D.hpp>
#include "../App.hpp"

class GameOverScene : public App::Scene
{
private:

    Font font = Font(150);
    double prevTime = Scene::Time();
    double time = 5.0f;
    std::shared_ptr<Table> table;

    Audio failedVoice = Audio(U"Asset/Voice/failed.mp3");
    Audio gameOverBGM= Audio(U"Asset/Bgm/bgm_gameover.mp3");

public:

    GameOverScene(const InitData& init)
        : IScene(init)
    {
        auto& sharedData = getData();
        table = sharedData.table;
        failedVoice.play();
        gameOverBGM.setVolume(0.2);
        gameOverBGM.play();
        table->boalEmission();
    }

    // 更新関数
    void update() override
    {
        auto now = Scene::Time();
        time -= (now - prevTime);
        prevTime = now;


        if (SimpleGUI::Button(U"もどる", Vec2(0, 0)))
        {
            gameOverBGM.stop();
            changeScene(U"Title", 300, true);
        }

        if (gameOverBGM.posSec() > 11.0f) {
            gameOverBGM.stop();
            changeScene(U"Standby", 300, true);
        }

        table->setServoAngle(SeesawToServoAngle(INIT_POS_X, INIT_POS_Y));

        table->bonusLED(5);
    }

    // 描画関数 (const 修飾)
    void draw() const override
    {
        font(U"ゲームオーバー").drawAt(Scene::Center());
    }
};