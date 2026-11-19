// SPDX-License-Identifier: MIT

#include "host/glfwHost.h"

#include <iostream>
#include <exception>

int main()
{
    try
    {
        monsterCatVisualizerOpenGL::host::GlfwHost host;
        return host.run();
    }
    catch (const std::exception& error)
    {
        std::cerr << "MonsterCatVisualizer: " << error.what() << std::endl;
        return 1;
    }
}
