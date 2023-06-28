#include "core.h"

int main()
{
    using namespace bblocks;

    core::init();

    while (core::is_running())
    {
        core::process_input();
        core::update();
        core::render();
    }
    
    core::shutdown();

    return 0;
}