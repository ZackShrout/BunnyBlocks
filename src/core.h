#pragma once

#include "common.h"

namespace bblocks::core
{
    bool init();
    void shutdown();
    void process_input();
    void update();
    void render();
    bool is_running();
}