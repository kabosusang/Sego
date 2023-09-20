#include <ThreadPool/imguiThread.h>

#include "VKimgui.h"

#include "Window/Window.h"

#include "Log/Log.h"

void glfwCallKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    SG_INFO("Keycode = :",key);
}


// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }

    std::unique_ptr<SegoWindow> Sego_Window = std::make_unique<SegoWindow>(window,1280,720);

    //Log
    Sego::Log::Log_Init(); //初始化日志
    SG_CORE_INFO("Hello World {0}",Sego_Window->GetHeight());


    //Imgui loop
    glfwSetKeyCallback(window,glfwCallKey);
    Init_Imgui(Sego_Window->GetWindow());

    
    // Main loop
    
    
    // Cleanup
    VkResult err = vkDeviceWaitIdle(g_Device);
    check_vk_result(err);
   
    Cleanup_imgui();
    Sego_Window->DestorySegowindow();

    //glfwDestroyWindow(window);
    //glfwTerminate();

    return 0;
}
