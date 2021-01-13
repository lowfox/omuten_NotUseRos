#pragma once
#include "../App.hpp"

class WakeUpScene : public App::Scene
{
public:

    WakeUpScene(const InitData& init)
        : IScene(init)
    {
        std::shared_ptr<Device<Seesaw::ReceiveCommand, Seesaw::SendCommand>> seesawXDevice = std::make_shared<Device<Seesaw::ReceiveCommand, Seesaw::SendCommand>>(0x02);
        std::shared_ptr<Device<Seesaw::ReceiveCommand, Seesaw::SendCommand>> seesawYDevice = std::make_shared<Device<Seesaw::ReceiveCommand, Seesaw::SendCommand>>(0x01);
        std::shared_ptr<Device<Table::ReceiveCommand, Table::SendCommand>> tableDevice = std::make_shared<Device<Table::ReceiveCommand, Table::SendCommand>>(0x04);

        seesawXDevice->connect();
        seesawYDevice->connect();
        tableDevice->connect();

        seesawXDevice->startReceive();
        seesawYDevice->startReceive();
        tableDevice->startReceive();

        getData().seesawX = std::make_shared<Seesaw>(seesawXDevice);
        getData().seesawY = std::make_shared<Seesaw>(seesawYDevice);
        getData().table   = std::make_shared<Table> (tableDevice);
    }

    void update() override
    {
        changeScene(U"Title", 0, true);
    }

    void draw() const override
    {

    }
};