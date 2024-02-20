#include "pch.h"
//input imgui
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

#include "SGData/UI/ini.h"
#include "Sg_UI.h"

Sg_UI* Sg_UI::ins = nullptr;
Sg_UI* UiContext = Sg_UI::getInstance();

QueueFamilyIndices Sg_UI::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    //检索队列 期望并且使用的队列
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,queueFamilies.data());
    //需要至少一个队列支持的familes
    int i = 0;
    for (const auto& queueFamily : queueFamilies){
        //找出GRAPHICS队列
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
        }
        //找出呈现队列
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device,i,surface,&presentSupport);
        if(presentSupport){
            indices.presentFamily = i;
        }

        if(indices.isComplete())
        {
            break; //提前退出
        }
       
        i++;
    }
    return indices;
}


void Sg_UI::Init_Sg_Imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    // Setup Dear ImGui style
    //ImGui::StyleColorsLight();

    io.Fonts->AddFontFromFileTTF("../SGData/Fonts/ziti.ttf",18,NULL,io.Fonts->GetGlyphRangesChineseFull());

    ImGui::GetIO().IniFilename = IMGUI_INI; //ImGui Layout 

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding = 4;
    style.FrameRounding = 4;
    style.GrabRounding = 3;
    style.ScrollbarSize = 7;
    style.ScrollbarRounding = 0;

    

    ImVec4* colors = style.Colors; 
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.36f, 0.36f, 102.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 171.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 255.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.45f, 0.45f, 0.45f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.17f, 0.17f, 0.17f, 0.95f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.78f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.51f, 0.51f, 0.51f, 0.70f);
	colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 0.97f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.40f, 0.13f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45f, 1.00f, 0.85f, 0.35f); 

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    queueIndices = FindQueueFamilies(physicalDevice);

     // Initialize some DearImgui specific resources
    createUIDescriptorPool();
    createUIRenderPass();
    createUICommandPool(&uiCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    createUICommandBuffers();
    createUIFramebuffers();

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);//需要检查的三种属性1.表面格式（像素格式、色彩空间
    MinImageCount = swapChainSupport.capabilities.minImageCount +1;
    // Provide bind points from Vulkan API
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance;
    init_info.PhysicalDevice = physicalDevice;
    init_info.Device = device;
    init_info.QueueFamily = queueIndices.graphicsFamily.value();
    init_info.Queue = graphicsqueue;
    init_info.DescriptorPool = uiDescriptorPool;
    init_info.MinImageCount = MinImageCount;
    init_info.ImageCount = MinImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = nullptr;

    ImGui_ImplVulkan_Init(&init_info, uiRenderPass);

    // Upload the fonts for DearImgui
    VkCommandBuffer commandBuffer_im = beginSingleTimeCommands(uiCommandPool);
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer_im);
    endSingleTimeCommands(commandBuffer_im, uiCommandPool);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
    vkDeviceWaitIdle(device);

    Init_ViewportRT();
}


void Sg_UI::createUIDescriptorPool()
{
    VkDescriptorPoolSize pool_sizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
    pool_info.pPoolSizes = pool_sizes;
    if (vkCreateDescriptorPool(device, &pool_info, nullptr, &uiDescriptorPool) != VK_SUCCESS) {
        SG_CORE_ERROR("Cannot allocate UI descriptor pool!");
    }

}

void Sg_UI::createUIRenderPass()
{
    // Create an attachment description for the render pass
    VkAttachmentDescription attachment = {};
    attachment.format = swapChainImageFormat;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Create a color attachment reference
    VkAttachmentReference attachmentReference = {};
    attachmentReference.attachment = 0;
    attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Create a subpass
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &attachmentReference;

    // Create a subpass dependency to synchronize our main and UI render passes
    // We want to render the UI after the geometry has been written to the framebuffer
    // so we need to configure a subpass dependency as such
    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Create external dependency
    subpassDependency.dstSubpass = 0; // The geometry subpass comes first
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0; // Wait on writes
    subpassDependency.dstStageMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Finally create the UI render pass
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &attachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &subpassDependency;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &uiRenderPass) != VK_SUCCESS) {
        SG_CORE_ERROR("Unable to create UI render pass!");
    }

}
void Sg_UI::createUICommandPool(VkCommandPool *cmdPool, VkCommandPoolCreateFlags flags)
{

auto indices = FindQueueFamilies(physicalDevice);

VkCommandPoolCreateInfo commandPoolCreateInfo = {};
commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
commandPoolCreateInfo.flags = flags;

    if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, cmdPool) != VK_SUCCESS) {
        SG_CORE_ERROR("Could not create graphics command pool!");
    }
}

void Sg_UI::createUICommandBuffers()
{
    uiCommandBuffers.resize(swapChainImageViews.size());

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = uiCommandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(uiCommandBuffers.size());

    if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, uiCommandBuffers.data()) != VK_SUCCESS) {
        SG_CORE_ERROR("Unable to allocate UI command buffers!");
    }

}

void Sg_UI::createUIFramebuffers()
{
    // Create some UI framebuffers. These will be used in the render pass for the UI
    uiFramebuffers.resize(swapChainImages.size());
    { // Create the UI framebuffers}
        VkImageView attachment[1];
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = uiRenderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = swapChainExtent.width;
        info.height = swapChainExtent.height;
        info.layers = 1;
        for (uint32_t i = 0; i < swapChainImages.size(); ++i) {
            attachment[0] = swapChainImageViews[i];
            if (vkCreateFramebuffer(device, &info, nullptr, &uiFramebuffers[i]) != VK_SUCCESS) {
                SG_CORE_ERROR("Unable to create UI framebuffers!");
            }
        }
    }
    
}


//交换链详情
SwapChainSupportDetails Sg_UI::querySwapChainSupport(VkPhysicalDevice device) 
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    //查询表面格式
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }
    //查询支持的演示模式
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }   
    return details;
}


//layout过度
VkCommandBuffer Sg_UI::beginSingleTimeCommands(VkCommandPool cmdPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = cmdPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer = {};
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

//endSingleTimeCommands
void Sg_UI::endSingleTimeCommands(VkCommandBuffer commandBuffer,VkCommandPool cmdPool) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsqueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsqueue);

    vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);
}


void Sg_UI::cleanupUIResources()
{
    for (auto framebuffer : uiFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    vkFreeCommandBuffers(device, uiCommandPool,
    static_cast<uint32_t>(uiCommandBuffers.size()), uiCommandBuffers.data());


}

void Sg_UI::cleanup()
{
    // Cleanup DearImGui
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

     vkDestroyDescriptorPool(device, uiDescriptorPool, nullptr);
    vkFreeCommandBuffers(device, uiCommandPool, static_cast<uint32_t>(uiCommandBuffers.size()),
                         uiCommandBuffers.data());
    vkDestroyCommandPool(device, uiCommandPool, nullptr);
    vkDestroyRenderPass(device, uiRenderPass, nullptr);

    for (auto framebuffer : uiFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    Destroy_ViewportRT(); //Clean Viewport
}

void Sg_UI::UpdataUiCleanDtata(VkFormat& swapformat,std::vector<VkImage>& swapimage,
std::vector<VkImageView>& swapimviews,VkExtent2D Extent)
{

    swapChainImageFormat = swapformat;
    swapChainImages = swapimage;
    swapChainImageViews = swapimviews;
    swapChainExtent = Extent;
    

    ImGui_ImplVulkan_SetMinImageCount(MinImageCount);
    createUICommandBuffers();
    createUIFramebuffers();

    Destroy_ViewportRT();
    Init_ViewportRT();
}


void Sg_UI::recordUICommands(uint32_t bufferIdx)
{
    VkCommandBufferBeginInfo cmdBufferBegin = {};
    cmdBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBegin.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(uiCommandBuffers[bufferIdx], &cmdBufferBegin) != VK_SUCCESS) {
        throw std::runtime_error("Unable to start recording UI command buffer!");
    }

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = uiRenderPass;
    renderPassBeginInfo.framebuffer = uiFramebuffers[bufferIdx];
    renderPassBeginInfo.renderArea.extent.width = swapChainExtent.width;
    renderPassBeginInfo.renderArea.extent.height = swapChainExtent.height;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(uiCommandBuffers[bufferIdx], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Grab and record the draw data for Dear Imgui
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), uiCommandBuffers[bufferIdx]);

    // End and submit render pass
    vkCmdEndRenderPass(uiCommandBuffers[bufferIdx]);

    if (vkEndCommandBuffer(uiCommandBuffers[bufferIdx]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffers!");
    }

}

#include "Vk_Image.h"

void Sg_UI::Init_ViewportRT()
{
    vulkan::resource::image::CreateImage(
    swapChainExtent.width,
    swapChainExtent.height,
    1,1,1,VK_IMAGE_TYPE_2D,false,VK_SAMPLE_COUNT_1_BIT,VK_FORMAT_B8G8R8A8_SRGB,
    VK_IMAGE_TILING_LINEAR,VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    ,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    this->viewportImage,this->viewportImageMemory);

    vulkan::resource::image::CreateImageView(
    this->viewportImage,VK_IMAGE_VIEW_TYPE_2D,VK_FORMAT_B8G8R8A8_SRGB,
    VK_IMAGE_ASPECT_COLOR_BIT,1,1,this->viewportImageView);

    vulkan::resource::image::CreateSampler(
    Vulkan_TextureFilter_Bilinear,Vulkan_TextureAddressing_Wrap,
    Vulkan_TextureBorderColor_OpaqueBlack,false,0.0f,0.0f,0.0f,this->viewportSampler);

}

void Sg_UI::CopytoViewport(uint32_t imageIndex,uint32_t currentFrame)
{
    //Copy To Viewport
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.image = app_device->swapChainImages[imageIndex];
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(app_device->commandBuffers[currentFrame], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                        0, 0, nullptr, 0, nullptr, 1, &barrier);

    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.image = viewportImage;

    vkCmdPipelineBarrier(app_device->commandBuffers[currentFrame], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                        0, 0, nullptr, 0, nullptr, 1, &barrier);

    VkImageCopy region = {};
    region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.srcSubresource.layerCount = 1;
    region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.dstSubresource.layerCount = 1;
    region.extent.width = app_device->swapChainExtent.width;
    region.extent.height = app_device->swapChainExtent.height;
    region.extent.depth = 1;

    vkCmdCopyImage(app_device->commandBuffers[currentFrame], 
                app_device->swapChainImages[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                viewportImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL ;
    barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    barrier.image = viewportImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(app_device->commandBuffers[currentFrame], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                        0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void Sg_UI::Destroy_ViewportRT()
{
    vkDestroyImage(device, viewportImage, nullptr);
    vkDestroyImageView(device, viewportImageView, nullptr);
    vkDestroySampler(device, viewportSampler, nullptr);
    vkFreeMemory(device, viewportImageMemory, nullptr);
}
