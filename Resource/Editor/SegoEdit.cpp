
#include "Window/Window.h"
#include <ThreadPool/imguiThread.h>
#include "Log/Log.h"
#include "Core/Application/Application.h"
#include "Device/SgDeviceAttri.h"

#include <iostream>

#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

int main(){



    // Main loop
    //Vulkan Renderer
    Application app;
 
    app.Run();


    return EXIT_SUCCESS;


}