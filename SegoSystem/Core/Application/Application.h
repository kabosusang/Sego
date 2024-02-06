#pragma once
#include "Events/Event.h"
#include "Window/Window.h"
#include "Renderer/SgUI/Sg_UI.h"
#include <memory>
#include "Events/WindowEvent.h"

class Application
{
public:
   	Application();
	virtual ~Application();

    void Run();
    //Event
    void OnEvent(Event& e);
    bool OnWindowClose(WindowCloseEvent& e);
    
    //Renderer
    void Application_DrawFrame(); //Vulkan循环Frame
    void drawUI(); //Imgui循环

    void updateUniformBuffer(uint32_t currentImage); //更新数据
    inline SegoWindow& GetWindow() {return *m_Window;}

    //RecreateSwapChain
    void RecreateSwapChain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    //处理窗口大小改变
    inline static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
    }


private:
    std::unique_ptr<SegoWindow> m_Window;//Main Window

    uint32_t currentFrame = 0;//帧索引
    bool framebufferResized = false;
    uint32_t MinImageCount = 0;
    bool m_Runing = true;
 

};