#include "Core/Application/Application.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"

#include "Renderer/SgUI/GUI.h"
#include "VKimgui.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//Main Window Instance
Application::Application()
{
    glfwInit();
    // Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
    }

    m_Window = std::make_unique<SegoWindow>(window,1280,720);
    m_Window->Init();

    m_Window->SetEventCallback(std::bind(&Application::OnEvent,this,std::placeholders::_1));

    //Log
    Sego::Log::Log_Init(); //初始化日志
    SG_CORE_INFO("Hello World {0} , {1}",m_Window->GetWidth(),m_Window->GetHeight());

    app_device.InputWindow(m_Window->GetWindow());
    app_device.SGvk_Device_Create_Instance();// ins
    app_device.setupDebugMessenger();
    app_device.SGvk_Device_Create_Surface();// sur
    app_device.SGvk_Device_Choose_Create_PhysicalDevice();//phy
    app_device.SGvk_Device_Create_LogicalDevice();//log
    app_device.SGvk_Device_Create_SwapChain();//swp
    app_device.SGvk_Device_Create_ImageViews();//img
    app_device.SGvk_Device_Create_RenderPass();//Ren
    app_device.SGvk_Device_Create_DescriptorSetLayout();//des
    app_device.SGvk_Device_Create_GraphicsPipeline("../SegoSystem/Renderer/shaders/vert.spv","../SegoSystem/Renderer/shaders/frag.spv");
    app_device.SGvk_Device_Create_DepthResources();
    app_device.SGvk_Device_Create_Framebuffers();
    app_device.SGvk_Device_Create_CommandPool();
    app_device.SGvk_Device_Create_TextureImage("../SegoSystem/Renderer/models/viking_room.png");
    app_device.SGvk_Device_Create_TextureImageView();
    app_device.SGvk_Device_Create_TextureSampler();
    loadModel("../SegoSystem/Renderer/models/viking_room.obj");
    app_device.SGvk_Device_Create_VertexBuffer();
    app_device.SGvk_Device_Create_IndexBuffer();
    app_device.SGvk_Device_Create_UniformBuffers();
    app_device.SGvk_Device_Create_DescriptorPool();
    app_device.SGvk_Device_Create_DescriptorSets();
    app_device.SGvk_Device_Create_CommandBuffer();
    app_device.SGvk_Device_Create_SyncObjects();

    Sg_ui.SgUI_Input(m_Window->GetWindow(),app_device.instance,app_device.surface,app_device.physicalDevice
    ,app_device.device,app_device.swapChainImageFormat,app_device.swapChainImages,app_device.swapChainImageViews
    ,app_device.swapChainExtent,app_device.graphicsqueue,app_device.presentQueue);
    Sg_ui.Init_Sg_Imgui(); //初始化imgui

}


void Application::Run()
{
    while (!glfwWindowShouldClose(m_Window->GetWindow()) && m_Runing){
        glfwPollEvents();
        drawUI();
        Application_DrawFrame();
    }
    vkDeviceWaitIdle(app_device.GetDevice());
    initnumber = true; //重置Imgui位置
}
#include "Events/KeyEvent.h"
#include "Camera/camera.h"
#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<KeyDownEvent>(GetcameraInput); //传入key摄像机
    dispatcher.Dispatch<KeyDownRepeate>(GetcameraRepeateInput); //传入key摄像机(重复输入)
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //cursor
    dispatcher.Dispatch<MouseMoveEvent>(GetCursorInput); //cursor move
    dispatcher.Dispatch<MouseScrollEvent>(GetCursorScrollInput);
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	//m_Runing = false;
	return true;
}


Application::~Application()
{
    app_device.cleanup();
    Sg_ui.cleanup();
    m_Window->DestorySegowindow();
}

void Application::Application_DrawFrame()
{

vkWaitForFences(app_device.device,1,&app_device.inFlightFences[currentFrame],VK_TRUE,UINT64_MAX);

uint32_t imageIndex;
VkResult result = vkAcquireNextImageKHR(app_device.device,app_device.swapChain,UINT64_MAX,app_device.imageAvailableSemaphores[currentFrame],VK_NULL_HANDLE,&imageIndex);
if(result == VK_ERROR_OUT_OF_DATE_KHR)//交换链已与 表面，不能再用于渲染。通常发生在调整窗口大小后
{
    g_SwapChainRebuild = true;
    app_device.recreateSwapChain();
    Sg_ui.cleanupUIResources();
    return ;
}else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
{
    SG_CORE_ERROR("failed to acquire swap chain image!");
}

// Record UI draw data
vkResetFences(app_device.device, 1, &app_device.inFlightFences[currentFrame]);

//recordUICommands(imageIndex);

vkResetCommandBuffer(app_device.commandBuffers[currentFrame], 0);
app_device.recordCommandBuffer(app_device.commandBuffers[currentFrame],imageIndex);

updateUniformBuffer(currentFrame);

//提交命令缓冲区
VkSubmitInfo submitInfo{};
submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

VkSemaphore waitSemaphores[] = {app_device.imageAvailableSemaphores[currentFrame]};//指定信号量
VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};//指定等待管道的哪个阶段
std::array<VkCommandBuffer, 2> cmdBuffers = {app_device.commandBuffers[imageIndex],Sg_ui.uiCommandBuffers[imageIndex],};
submitInfo.waitSemaphoreCount = 1;
submitInfo.pWaitSemaphores = waitSemaphores;
submitInfo.pWaitDstStageMask = waitStages;

submitInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());
submitInfo.pCommandBuffers = cmdBuffers.data();

VkSemaphore signalSemaphores[] = {app_device.renderFinishedSemaphores[currentFrame]};
submitInfo.signalSemaphoreCount = 1;
submitInfo.pSignalSemaphores = signalSemaphores;

VkResult err;
err = vkQueueSubmit(app_device.graphicsqueue, 1, &submitInfo, app_device.inFlightFences[currentFrame]);
if (err!= VK_SUCCESS) {
    std::cout << "Error Code:" << err ;
   SG_CORE_ERROR("failed to submit draw command buffer!");
    
}//将命令缓冲区提交到图形队列

//将结果提交回交换链 让它最终出现在屏幕上
VkPresentInfoKHR presentInfo{};
presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

presentInfo.waitSemaphoreCount = 1;
presentInfo.pWaitSemaphores = signalSemaphores;//渲染完成触发该信号量

VkSwapchainKHR swapChains[] = {app_device.swapChain};
presentInfo.swapchainCount = 1;
presentInfo.pSwapchains = swapChains;
presentInfo.pImageIndices = &imageIndex;

presentInfo.pResults = nullptr; // Optional 

result = vkQueuePresentKHR(app_device.presentQueue, &presentInfo);//真正提交到交换链
if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR
|| framebufferResized) {
    framebufferResized = false;
    app_device.recreateSwapChain();
    Sg_ui.cleanupUIResources();//清理
} else if (result != VK_SUCCESS) {
    SG_CORE_ERROR("failed to present swap chain image!");
}//如果交换链不是最佳的需要重新创建交换链


//前进下一帧
currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

}



void Application::drawUI()
{
    // Resize swap chain?
    
        if (g_SwapChainRebuild)
        {
            int width, height;
            glfwGetFramebufferSize(m_Window->GetWindow(), &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(app_device.instance, app_device.physicalDevice, app_device.device, &g_MainWindowData, Sg_ui.queueIndices.graphicsFamily.value(), nullptr, width, height, MinImageCount);
                g_MainWindowData.FrameIndex = 0;
                g_SwapChainRebuild = false;
            }
        }

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
   // ImGui::DockSpaceOverViewport(); 使得imgui主窗口停靠但是渲染被遮挡了

    DrawGUI(app_device.wd);
 
    // Render Dear ImGui
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        GLFWwindow* backuo_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backuo_current_context);
}
}


#include "Camera/camera.h"
//更新统一数据
void Application::updateUniformBuffer(uint32_t currentImage)
{
static auto startTime = std::chrono::high_resolution_clock::now();

auto currentTime = std::chrono::high_resolution_clock::now();
float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

UniformBufferObject ubo{};
ubo.model = glm::mat4(1.0f);
ubo.view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
ubo.proj = glm::perspective(glm::radians(fov), app_device.swapChainExtent.width / (float) app_device.swapChainExtent.height, 0.1f, 10.0f);
ubo.proj[1][1] *= -1;

memcpy(app_device.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

