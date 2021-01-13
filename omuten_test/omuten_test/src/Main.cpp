#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include <thread>
#include "App.hpp"
#include "Scenes/Title.hpp"
#include "Scenes/Ready.hpp"
#include "Scenes/WakeUp.hpp"
#include "Scenes/Game.hpp"
#include "Scenes/GameOver.hpp"
#include "Scenes/Standby.hpp"
#include "Scenes/GameClear.h"
void Main()
{
    Window::Resize(Size(1280, 720));
   // Window::SetFullscreen(true);
    FontAsset::Register(U"TitleFont", 150, Typeface::Heavy);
    FontAsset::Register(U"commonFont", 50, Typeface::Heavy);

    App manager;

    manager.add<WakeUpScene>(U"WakeUp");
    manager.add<TitleScene>(U"Title");
    manager.add<StandbyScene>(U"Standby");
    manager.add<ReadyScene>(U"Ready");
    manager.add<GameScene>(U"Game");
    manager.add<GameClearScene>(U"GameClear");
    manager.add<GameOverScene>(U"GameOver");

    manager.init(U"WakeUp");

    while (System::Update())
    {
        if (!manager.update())
        {
            break;
        }
    }

    return;
}
