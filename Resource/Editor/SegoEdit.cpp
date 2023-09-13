#include <ThreadPool/imguiThread.h>

#include "VKimgui.h"

#include <Log/Log.h>

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

    //Log
    Sego::Log::Log_Init(); //初始化日志
    SG_CORE_INFO("Hello World {}","Log Start");

    //Imgui loop
    glfwSetKeyCallback(window,glfwCallKey);
    Init_Imgui(window);

    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        


        /* code */
    }
    
    
    // Cleanup
    VkResult err = vkDeviceWaitIdle(g_Device);
    check_vk_result(err);
   
    Cleanup_imgui();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
