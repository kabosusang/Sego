#include "pch.h"

#include "Core/Application/Application.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "VK_Global_Data.h"
#include "Editor/VKimgui.h"

#include "SGComponent/game_object.h"
#include "SGComponent/component.h"
#include "SGComponent/transform.h"
#include "SGComponent/MeshFilter/MeshFilter.h"
#include "SGComponent/MeshRenderer/MeshRender.h"
#include "SGComponent/LightRenderer/LightRenderer.h"
//Main Window Instance
#include "Object/object.h"
#include "Core/Material/Material.h"

#include "Editor/UiWindow.h"

Application::Application()
{
    glfwInit();
    // Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1800, 1200, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
    }

    m_Window = std::make_unique<SegoWindow>(window,1800,1200);
    m_Window->Init();

    m_Window->SetEventCallback(std::bind(&Application::OnEvent,this,std::placeholders::_1));
    glfwSetFramebufferSizeCallback(m_Window->GetWindow(), framebufferResizeCallback);

    //Log
    Sego::Log::Log_Init(); //初始化日志
    SG_CORE_INFO("Hello World {0} , {1}",m_Window->GetWidth(),m_Window->GetHeight());

    app_device->InputWindow(m_Window->GetWindow());
    app_device->InitVulkan();

    //Create Resource
    app_device->SGvk_Device_Create_DescriptorPool();
    app_device->SGvk_Device_Create_CommandBuffer();
    app_device->SGvk_Device_Create_SyncObjects();
    
    UiContext->SgUI_Input(m_Window->GetWindow(),app_device->instance,app_device->surface,g_physicalDevice
    ,g_device,app_device->swapChainImageFormat,app_device->swapChainImages,app_device->swapChainImageViews
    ,app_device->swapChainExtent,app_device->graphicsqueue,app_device->presentQueue);
    UiContext->Init_Sg_Imgui(); //初始化imgui
    MinImageCount = app_device->image_count;
    
    //初始化UI class
    int width,height;
    glfwGetWindowPos(m_Window->GetWindow(),&width,&height);
    m_Window->SetWindowPos(width,height);
    

    //创建GameObject(Viki-room)
    GameObject* go = new GameObject("GO");
    //挂上 Transform
    transform_obj.push_back(dynamic_cast<Transform*>(go->AddComponent("Transform")));
    transform_obj[0]->set_position(glm::vec3(3.0f,0.0f,0.0f));
    transform_obj[0]->set_rotation(glm::vec3(0.0f,0.0f,0.0f));
    transform_obj[0]->set_scale(glm::vec3(1.0f));
   
    //挂上MeshFilter
    auto meshfilter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
    meshfilter->LoadMesh(SG_DATA_PATH("Model/obj/viking_room.obj"));
    
    //挂上MeshRenderer 组件
    mesh_renderer.push_back(dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer")));
    Material* material=new Material();//设置材质
    material->Parse(SG_DATA_PATH("Material/viking_room.mat"));
    mesh_renderer[0]->SetMaterial(material);
    

    //创建GameObject(Light Cube)
    GameObject* go2 = new GameObject("GO");
    //挂上 Transform
    transform_obj.push_back(dynamic_cast<Transform*>(go2->AddComponent("Transform")));
    transform_obj[1]->set_position(glm::vec3(1.0f,5.0f,0.0f));
    transform_obj[1]->set_rotation(glm::vec3(0.0f,0.0f,0.0f));
    transform_obj[1]->set_scale(glm::vec3(1.0f));
   
    //挂上MeshFilter
    auto meshfilter_2 = dynamic_cast<MeshFilter*>(go2->AddComponent("MeshFilter"));
    meshfilter_2->LoadMesh(SG_DATA_PATH("BasicShapes/Light/Light.obj"));
   
    //挂上MeshRenderer 组件
    mesh_renderer.push_back(dynamic_cast<MeshRenderer*>(go2->AddComponent("MeshRenderer")));
    Material* material_2=new Material();//设置材质
    material_2->Parse(SG_DATA_PATH("Material/Light/Light.mat"));
    mesh_renderer[1]->SetMaterial(material_2);


    //创建GameObject(Viki-room2)
    GameObject* go3 = new GameObject("GO");
    //挂上 Transform
    transform_obj.push_back(dynamic_cast<Transform*>(go3->AddComponent("Transform")));
    transform_obj[2]->set_position(glm::vec3(-2.0f,0.0f,0.0f));
    transform_obj[2]->set_rotation(glm::vec3(0.0f,0.0f,0.0f));
    transform_obj[2]->set_scale(glm::vec3(1.0f));
   
    //挂上MeshFilter
    auto meshfilter_3 = dynamic_cast<MeshFilter*>(go3->AddComponent("MeshFilter"));
    meshfilter_3->LoadMesh(SG_DATA_PATH("Model/obj/viking_room.obj"));
    
    //挂上MeshRenderer 组件
    mesh_renderer.push_back(dynamic_cast<MeshRenderer*>(go3->AddComponent("MeshRenderer")));
    Material* material_3=new Material();//设置材质
    material_3->Parse(SG_DATA_PATH("Material/blend.mat"));
    mesh_renderer[2]->blend = true;
    mesh_renderer[2]->SetMaterial(material_3);


    //创建灯光场景测试:
    //创建GameObject(Pot)
    GameObject* go4 = new GameObject("Light");
    //挂上 Transform
    transform_obj.push_back(dynamic_cast<Transform*>(go4->AddComponent("Transform")));
    transform_obj[3]->set_position(glm::vec3(-1.0f,2.0f,0.0f));
    transform_obj[3]->set_rotation(glm::vec3(0.0f,0.0f,0.0f));
    transform_obj[3]->set_scale(glm::vec3(1.0f));
   
    //挂上MeshFilter
    auto meshfilter_4 = dynamic_cast<MeshFilter*>(go4->AddComponent("MeshFilter"));
    meshfilter_4->LoadMesh(SG_DATA_PATH("Model/obj/pot.obj"));
    
    //挂上MeshRenderer 组件
    Light_renderer.push_back(dynamic_cast<LightRenderer*>(go4->AddComponent("LightRenderer")));
    Material* material_4=new Material();//设置材质
    material_4->Parse(SG_DATA_PATH("Material/pot.mat"));
    Light_renderer[0]->SetMaterial(material_4);


    //创建GameObject(Plane)
    GameObject* go5 = new GameObject("Light");
    //挂上 Transform
    transform_obj.push_back(dynamic_cast<Transform*>(go5->AddComponent("Transform")));
    transform_obj[4]->set_position(glm::vec3(0.0f,-1.0f,0.0f));
    transform_obj[4]->set_rotation(glm::vec3(0.0f,0.0f,0.0f));
    transform_obj[4]->set_scale(glm::vec3(1.0f));
   
    //挂上MeshFilter
    auto meshfilter_5 = dynamic_cast<MeshFilter*>(go5->AddComponent("MeshFilter"));
    meshfilter_5->LoadMesh(SG_DATA_PATH("BasicShapes/Plane/Plane.obj"));
    
    //挂上MeshRenderer 组件
    Light_renderer.push_back(dynamic_cast<LightRenderer*>(go5->AddComponent("LightRenderer")));
    Material* material_5=new Material();//设置材质
    material_5->Parse(SG_DATA_PATH("Material/plane/plane.mat"));
    Light_renderer[1]->SetMaterial(material_5);





    //创建相机Object
    auto go_camera = new GameObject("main_camera");
    
    //挂上 Transform组件
    transform_camera = dynamic_cast<Transform*>(go_camera->AddComponent("Transform"));
    transform_camera->set_position(glm::vec3(0.0f,0.0f,19.0f));
    //挂上Camera组件
    camera =dynamic_cast<Camera*>(go_camera->AddComponent("Camera"));
    
   

}

void Application::Run()
{
    while (!glfwWindowShouldClose(m_Window->GetWindow()) && m_Runing){
        glfwPollEvents();
        drawUI();
        Application_DrawFrame();
    }
   
}
#include "Events/KeyEvent.h"
#include "SGComponent/Camera/camera.h"
#include "Application.h"
#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
	//dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<KeyDownEvent>(GetcameraInput); //传入key摄像机
    dispatcher.Dispatch<KeyDownRepeate>(GetcameraRepeateInput); //传入key摄像机(重复输入)
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //cursor
    dispatcher.Dispatch<MouseMoveEvent>(GetCursorInput); //cursor move
    dispatcher.Dispatch<MouseScrollEvent>(GetCursorScrollInput);
    //mousedown
    dispatcher.Dispatch<MouseButtonPressedEvent>(GetMouseDownInput);
    //mouserelease
    dispatcher.Dispatch<MouseButtonReleasedEvent>(GetMouseReleaseInput);

}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	//m_Runing = false;
	return true;
}


Application::~Application()
{
    app_device->cleanup();
    UiContext->cleanup();
    m_Window->DestorySegowindow();
 
}


void Application::Application_DrawFrame()
{
//Fence
vkWaitForFences(g_device,1,&app_device->inFlightFences[currentFrame],VK_TRUE,UINT64_MAX);
uint32_t imageIndex;
VkResult result = vkAcquireNextImageKHR(g_device,app_device->swapChain,UINT64_MAX,app_device->imageAvailableSemaphores[currentFrame],
VK_NULL_HANDLE,&imageIndex);

if(result == VK_ERROR_OUT_OF_DATE_KHR)//交换链已与 表面，不能再用于渲染。通常发生在调整窗口大小后
{
    g_SwapChainRebuild = true;
    RecreateSwapChain();
    return ;
}else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
{
    SG_CORE_ERROR("failed to acquire swap chain image!");
}

vkResetFences(g_device, 1, &app_device->inFlightFences[currentFrame]);
// Record UI draw data
UiContext->recordUICommands(imageIndex);

//CommandBuffers
vkQueueWaitIdle(app_device->graphicsqueue);
vkResetCommandBuffer(app_device->commandBuffers[currentFrame], 0);
recordCommandBuffer(app_device->commandBuffers[currentFrame],imageIndex);

//主渲染Renderer
for(auto& mesh : mesh_renderer)
{
// 结束渲染通道
mesh->Render(app_device->commandBuffers[currentFrame],imageIndex);
}
for(auto& Light : Light_renderer)
{
// 结束渲染通道
Light->Render(app_device->commandBuffers[currentFrame],imageIndex);
}

UiContext->CopytoViewport(imageIndex,currentFrame);

vkCmdEndRenderPass(app_device->commandBuffers[currentFrame]);
if (vkEndCommandBuffer(app_device->commandBuffers[currentFrame]) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to record command buffer!");
}

updateUniformBuffer(currentFrame);

//提交命令缓冲区
VkSubmitInfo submitInfo{};
submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

VkSemaphore waitSemaphores[] = {app_device->imageAvailableSemaphores[currentFrame]};//指定信号量
VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};//指定等待管道的哪个阶段
std::array<VkCommandBuffer, 2> cmdBuffers = {app_device->commandBuffers[imageIndex],UiContext->uiCommandBuffers[imageIndex],};
submitInfo.waitSemaphoreCount = 1;
submitInfo.pWaitSemaphores = waitSemaphores;
submitInfo.pWaitDstStageMask = waitStages;

submitInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());
submitInfo.pCommandBuffers = cmdBuffers.data();

VkSemaphore signalSemaphores[] = {app_device->renderFinishedSemaphores[currentFrame]};
submitInfo.signalSemaphoreCount = 1;
submitInfo.pSignalSemaphores = signalSemaphores;

VkResult err;
err = vkQueueSubmit(app_device->graphicsqueue, 1, &submitInfo, app_device->inFlightFences[currentFrame]);
if (err!= VK_SUCCESS) {
    std::cout << "Error Code:" << err ;
   SG_CORE_ERROR("failed to submit draw command buffer!");
}//将命令缓冲区提交到图形队列
 

//将结果提交回交换链 让它最终出现在屏幕上
VkPresentInfoKHR presentInfo{};
presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

presentInfo.waitSemaphoreCount = 1;
presentInfo.pWaitSemaphores = signalSemaphores;//渲染完成触发该信号量

VkSwapchainKHR swapChains[] = {app_device->swapChain};
presentInfo.swapchainCount = 1;
presentInfo.pSwapchains = swapChains;
presentInfo.pImageIndices = &imageIndex;
presentInfo.pResults = nullptr; // Optional 


result = vkQueuePresentKHR(app_device->presentQueue, &presentInfo);//真正提交到交换链
if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR
|| framebufferResized) {
    g_SwapChainRebuild = false;
    RecreateSwapChain(); //
} else if (result != VK_SUCCESS) {
    SG_CORE_ERROR("failed to present swap chain image!");
}//如果交换链不是最佳的需要重新创建交换链

//前进下一帧
currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    //camera相关
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}

//IMGUI
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
                ImGui_ImplVulkanH_CreateOrResizeWindow(app_device->instance, g_physicalDevice, g_device, &g_MainWindowData, UiContext->queueIndices.graphicsFamily.value(), nullptr, width, height, MinImageCount);
                g_MainWindowData.FrameIndex = 0;
                g_SwapChainRebuild = false;
            }
            
        }

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::DockSpaceOverViewport(); 
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
    Editor::UiWindow::Draw();

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


#include "Lights/LightConstans.h"
//更新统一数据
void Application::updateUniformBuffer(uint32_t currentImage)
{   
//camera
//transform_camera->set_position(cameraPos);
camera->SetView(cameraFront,cameraUp);
camera->SetProjection(fov,app_device->swapChainExtent.width / (float) app_device->swapChainExtent.height, 0.1f, 100.0f);

for (auto mesh_obj:mesh_renderer)
{
UniformBufferObject ubo{};
ubo.model = mesh_obj->Model_mat4();
ubo.view = camera->view_mat4();
//ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
ubo.proj = camera->projection_mat4();
ubo.proj[1][1] *= -1;
memcpy(mesh_obj->GetuniformBuffersMapped()[currentImage], &ubo, sizeof(ubo));

}

for (auto Light_obj:Light_renderer)
{
UniformBufferObject ubo{};
ubo.model = Light_obj->Model_mat4();
ubo.view = camera->view_mat4();
//ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
ubo.proj = camera->projection_mat4();
ubo.proj[1][1] *= -1;
memcpy(Light_obj->GetuniformBuffersMapped_obj()[currentImage], &ubo, sizeof(ubo));

//viewpos
phone.viewpos = transform_camera->position();
memcpy(Light_obj->GetuniformBuffersMapped_light()[currentImage],&phone,sizeof(phone));
}

}

void Application::RecreateSwapChain()
{
    app_device->recreateSwapChain();
    for(auto& mesh : mesh_renderer)
    {
        mesh->RecreatePipeline();
    }
    for(auto& light : Light_renderer)
    {
        light->RecreatePipeline();
    }

    UiContext->cleanupUIResources();
    UiContext->UpdataUiCleanDtata(app_device->swapChainImageFormat,app_device->swapChainImages
    ,app_device->swapChainImageViews,app_device->swapChainExtent);
}

//RecordCommand
void Application::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
VkCommandBufferBeginInfo beginInfo{};
beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
beginInfo.flags = 0; // Optional
beginInfo.pInheritanceInfo = nullptr; // Optional 继承信息

if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to begin recording command buffer!");
}
//启用渲染通道
VkRenderPassBeginInfo renderPassInfo{};
renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
renderPassInfo.renderPass = app_device->renderPass;
renderPassInfo.framebuffer = app_device->swapChainFramebuffers[imageIndex];

renderPassInfo.renderArea.offset = {0,0};
renderPassInfo.renderArea.extent = app_device->swapChainExtent;
//清除值
// 清除模板缓冲区
std::array<VkClearValue, 2> clearColor = {};
clearColor[0].color = {{0.0f,0.0f,0.0f,1.0f}};
clearColor[1].depthStencil = {1.0f,0};

renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColor.size());
renderPassInfo.pClearValues = clearColor.data();

vkCmdBeginRenderPass(commandBuffer,&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);//开始渲染通道

//视口与剪刀(可以定为动态状态或者静态状态)
    VkViewport viewport{};
    viewport.x = 0.0f,
    viewport.y = 0.0f;
    viewport.width = (float)app_device->swapChainExtent.width;
    viewport.height = (float)app_device->swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer,0,1,&viewport);
    //我们选择一个完整的剪刀矩形
    VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = app_device->swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
