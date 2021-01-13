#pragma once
#include <Siv3D.hpp>
#include "../App.hpp"

class GameClearScene : public App::Scene
{
private:

    Font font = Font(150);
    std::shared_ptr<Table> table;

    Audio clearVoice = Audio(U"Asset/Voice/clear.mp3");
    Audio gameClearBGM= Audio(U"Asset/Bgm/bgm_clear.mp3");
    Audio goalOut= Audio(U"Asset/Se/goalout.mp3");

    double prevTime= Scene::Time();
    double time = 10.0f;


public:

    GameClearScene(const InitData& init)
        : IScene(init)
    {
        auto& sharedData = getData();
        table = sharedData.table;
        clearVoice.play();
        gameClearBGM.setVolume(0.2);
        goalOut.setVolume(0.8);
        gameClearBGM.play();
    }

    void update() override
    {
        auto now = Scene::Time();
        time -= (now - prevTime);
        prevTime = now;
       // Print << U"time:" << time;


       table->setServoAngle(SeesawToServoAngle(INIT_POS_X, INIT_POS_Y));

        
       if (time < 8.0f && table->isGoal()) {
           table->boalEmission();
        //   Print << U"boalEmission";
           
           goalOut.play();

       }

        if (time < 6.0f) {
//            table->boalEmission_init();
         //  Print << U"boalEmission_init";
        }
 
        if (SimpleGUI::Button(U"もどる", Vec2(0, 0)))
        {
            gameClearBGM.stop();
            changeScene(U"Title", 300, true);
        }

        if (gameClearBGM.posSec() > 11.0f ) {
            gameClearBGM.stop();
            changeScene(U"Standby", 300, true);
        }
        table->bonusLED(0);
    }

    void draw() const override
    {
        font(U"ゲームクリア！").drawAt(Scene::Center());
    }
};