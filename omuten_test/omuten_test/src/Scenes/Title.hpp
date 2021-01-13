#pragma once
#include "../App.hpp"

class TitleScene : public App::Scene
{
private:

    Font font = Font(50);

public:

    TitleScene(const InitData& init)
        : IScene(init)
    {
    }

    void update() override
    {
        if (SimpleGUI::Button(U"������", Vec2(Scene::Center().x, 300)))
        {
            changeScene(U"Standby", 0, false);
        }
    }

    void draw() const override
    {
       
        //font(Siv3DVersion).draw();
        FontAsset(U"TitleFont")(U"�R���R���c�C���Y").drawAt(Window::ClientCenter().x, 150);
        font(U"Hello, Siv3D!").drawAt(Scene::Center(), Palette::White);
        font(U"SeesawX�@{} Angle {}"_fmt(getData().seesawX->getDevice()->isConnect() ? U"�ڑ��ς�": U"���ڑ�",getData().seesawX->getAngle())).draw(50, 400);
        font(U"SeesawY�@{} Angle {}"_fmt(getData().seesawY->getDevice()->isConnect() ? U"�ڑ��ς�" : U"���ڑ�", getData().seesawY->getAngle())).draw(50, 500);
        font(U"Table�@�@{}"_fmt(getData().table->getDevice()->isConnect() ? U"�ڑ��ς�" : U"���ڑ�")).draw(50, 600);
    }
};