#include <ThreadPool/imguiThread.h>
#include "VKimgui.h"
#include "Window/Window.h"
#include "Log/Log.h"
#include "Renderer/Vulkan/VulkanRenderer.h"

#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

int main(){

    if (!glfwInit())
        return 1;

    // Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    if(true)
    {
        //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);//创建无边框窗口
        //glfwWindowHint(GLFW_TITLEBAR,false);

    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }

    std::unique_ptr<SegoWindow> Sego_Window = std::make_unique<SegoWindow>(window,1280,720);
    Sego_Window->Init();
   

    //Log
    Sego::Log::Log_Init(); //初始化日志
    SG_CORE_INFO("Hello World {0}",Sego_Window->GetHeight());


    //Imgui loop
    
    //Init_Imgui(Sego_Window->GetWindow());
    // Main loop
    
    //Vulkan Renderer
    HelloTriangleApplication app;

    app.InputWindow(Sego_Window->GetWindow());

    Sego_Window->SetEventCallback(std::bind(&HelloTriangleApplication::OnEvent,&app,std::placeholders::_1));

    //Imugi Renderer
    
    try{
        app.run();
    }catch(const std::exception&e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } 
    

    
    Sego_Window->DestorySegowindow();

    //glfwDestroyWindow(window);
    //glfwTerminate();
    return EXIT_SUCCESS;


}