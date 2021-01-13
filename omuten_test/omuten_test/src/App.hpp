#pragma once
#include <Siv3D.hpp>
#include "Device.hpp"
#include "Seesaw.hpp"
#include "Table.hpp"

struct ShareData {
    std::shared_ptr<Seesaw> seesawX;
    std::shared_ptr<Seesaw> seesawY;
    std::shared_ptr<Table> table;
};

using App = SceneManager<String, ShareData>;
