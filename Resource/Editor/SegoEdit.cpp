
#include "Window/Window.h"
#include <ThreadPool/imguiThread.h>
#include "Log/Log.h"
#include "Core/Application/Application.h"
#include "Device/device.h"

#include <iostream>

#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

int main(){

   int monitorCount;
    //GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    GLFWmonitor** pMonitor =  glfwGetMonitors(&monitorCount);

    //device monitor
    std::vector<MonitorAttribute> devMonitor(monitorCount);
    std::cout << "Now, Screen number is " << monitorCount << std::endl;
    for(int i=0; i<monitorCount; i++){
        int screen_x, screen_y;
        const GLFWvidmode * mode = glfwGetVideoMode(pMonitor[i]);
        std::cout << "Screen size is X = " << mode->width << ", Y = " << mode->height << std::endl;
        devMonitor[monitorCount].MonitorWidth = mode->width;
        devMonitor[monitorCount].MonitorHeight = mode->height;
        devMonitor[monitorCount].MonitorIndex = monitorCount;
    }


    // Main loop
    //Vulkan Renderer
    Application app;
 
    app.Run();


    return EXIT_SUCCESS;


}