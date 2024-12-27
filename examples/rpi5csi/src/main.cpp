#include "camera/interfaces/rpi5/csi.hpp"
#include "log/interfaces/console.hpp"

#include <iostream>

int main()
{
    try
    {
        uint32_t camnum = 0, width = 640, height = 480, fps = 30;
        auto loglvl = logging::type::info;
        auto logIf = logging::LogFactory::create<logging::console::Log>(loglvl);
        auto camera = camera::Factory::create<camera::csi::Camera>(
            logIf, {camnum, width, height, fps});
        camera->start();
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
