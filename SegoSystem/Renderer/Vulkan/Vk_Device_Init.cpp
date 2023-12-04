#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "Log/Log.h"

//C++
#include <iostream>

#include <optional>
#include <set>
#include <string>


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

//检查列表是否存在Layer
bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
            layerFound = true;
            break;
        }
    }
    if (!layerFound) {
        return false;
    }
    return true;
}
}

//返回基于是否启用验证层
std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}



/** Choose Function
 * SGvk_Device_Choose_QueueFamilies
 * Vk_Device_Status_PhysicalDevice
 * Vk_Device_Status_CheckDeviceExtensionSupport
 * Vk_Device_Status_QuerySwapChainSupport
 * SGvk_Device_Choose_SwapSurfaceFormat
 * SGvk_Device_Choose_SwapPresentMode
 * SGvk_Device_Choose_Choose_SwapExtent
 * SGvk_Device_Choose_FindDepthFormat
 * 
 * 
 * 
 * 
 * 
*/
//寻找队列

QueueFamilyIndices Application_Device::SGvk_Device_Choose_QueueFamilies(VkPhysicalDevice device)
{
     QueueFamilyIndices indices;
    //检索队列 期望并且使用的队列
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    std::cout <<"Has queue " << queueFamilyCount<<std::endl;
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
//检查物理设备是否适合我们想要执行的操作
bool Application_Device::Vk_Device_Status_PhysicalDevice(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;  //显卡详细信息
    VkPhysicalDeviceFeatures  deviceFeatures;//查询各种特性
    
    vkGetPhysicalDeviceProperties(device,&deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    std::cout <<"Your GPU is " << deviceProperties.deviceName << std::endl;
   
    QueueFamilyIndices indices = SGvk_Device_Choose_QueueFamilies(device);
    
    //检查是否支持交换链
    bool extensionsSupported = Vk_Device_Status_CheckDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
    //检查SwapChainSupportDetails结构体是否填充
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = Vk_Device_Status_QuerySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && deviceFeatures.samplerAnisotropy && swapChainAdequate; 
}
//检查是否支持交换链
bool Application_Device::Vk_Device_Status_CheckDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

//交换链详情
SwapChainSupportDetails Application_Device::Vk_Device_Status_QuerySwapChainSupport(VkPhysicalDevice device) 
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
//choose best SurfaceFormat
VkSurfaceFormatKHR Application_Device::SGvk_Device_Choose_SwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
{
    for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
    }
}
    return availableFormats[0];//如果不满足上面的条件则默认选择第一个

}

//choose best presentMode
VkPresentModeKHR Application_Device::SGvk_Device_Choose_SwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR) {
            return availablePresentMode;
        }
    }
    //VK_PRESENT_MODE_MAILBOX_KHR为解锁上线 
    //VK_PRESENT_MODE_FIFO_KHR为垂直同步
    return VK_PRESENT_MODE_FIFO_KHR;//改参数任何设备都可以使用虽然不如上面的好
}

//choose best SwapExtent(交换范围)
VkExtent2D Application_Device::SGvk_Device_Choose_Choose_SwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(wd, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

//辅助选择具有深度的组件格式
VkFormat Application_Device::SGvk_Device_Choose_FindDepthFormat() {
    return SGvk_Device_Choose_FindSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}
VkFormat Application_Device::SGvk_Device_Choose_FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates){
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice,format,&props);

       if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
         return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
        return format;
        }
    }
    SG_CORE_ERROR("failed to find supported format!");

}

/** Create Function
 * SGvk_Device_Create_Instance
 * SGvk_Device_Create_Surface
 * SGvk_Device_Choose_Create_PhysicalDevice
 * SGvk_Device_Create_LogicalDevice
 * SGvk_Device_Create_SwapChain
 * SGvk_Device_Create_ImageViews  SGvk_Device_Create_ImageView_AttachFuc
 * SGvk_Device_Create_RenderPass
 * SGvk_Device_Create_DescriptorSetLayout
 * SGvk_Device_Create_GraphicsPipeline
 * SGvk_Device_Create_ShaderModule
 * SGvk_Device_Create_DepthResources
 * SGvk_Device_Create_Image   findMemoryType
 * SGvk_Device_Create_ImageView  
 * SGvk_Device_Create_Framebuffers
 * SGvk_Device_Create_CommandPool
 * SGvk_Device_Create_TextureImage SGvk_Device_Create_Buffer
 * SGvk_Device_Create_TextureImageView
 * SGvk_Device_Create_TransitionImageLayout
 * SGvk_Device_Create_TextureSampler
 * SGvk_Device_Create_VertexBuffer
 * SGvk_Device_Create_IndexBuffer
 * SGvk_Device_Create_UniformBuffers
 * SGvk_Device_Create_DescriptorPool
 * SGvk_Device_Create_DescriptorSets
 * SGvk_Device_Create_CommandBuffer
 * SGvk_Device_Create_SyncObjects
 * 
*/
//ins
void Application_Device::SGvk_Device_Create_Instance()
{
   if(enableValidationLayers && !checkValidationLayerSupport()){
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    //告诉我们要使用的全局扩展和验证层的Vulkan 驱动程序
    auto extensions = getRequiredExtensions();//使用消息回调扩展
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};//变量放置在 if 语句之外，以确保在 vkCreateInstance 调用之前不会销毁它
    
    //添加验证层
    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    
        populateDebugMessengerCreateInfo(debugCreateInfo);//为了让验证层涵盖到创建instance
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }else{
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
   
    VkResult result = vkCreateInstance(&createInfo,nullptr,&instance);
    if(result != VK_SUCCESS){
        throw std::runtime_error("failed to create instance!");
    }
}


//设置DebugMessenger
void Application_Device::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}


//sur
void Application_Device::SGvk_Device_Create_Surface()
{
    VkResult res= glfwCreateWindowSurface(instance, wd, nullptr, &surface);
   if (res != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create window surface! Error code:{0}",res);
}
}

//pick Physical
void Application_Device::SGvk_Device_Choose_Create_PhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if(deviceCount == 0){
        SG_CORE_ERROR("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance,&deviceCount,devices.data());

    for (const auto& device : devices )
    {
        if(Vk_Device_Status_PhysicalDevice(device))
        {
            physicalDevice = device;
            break;
        }
    }
    if(physicalDevice == VK_NULL_HANDLE){
       SG_CORE_ERROR("failed to find a suitable GPU!");
    }
    
}
//log device
void Application_Device::SGvk_Device_Create_LogicalDevice()
{
    QueueFamilyIndices indices = SGvk_Device_Choose_QueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),indices.presentFamily.value()};
   
    float queuePriority = 1.0f; //prior
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    //指定使用的设备功能
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;//各项异性过滤
    //逻辑设备结构体填写
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
  
    createInfo.pEnabledFeatures = &deviceFeatures;
    //启用设备扩展
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
//创建逻辑设备
if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create logical device!");
}
//队列与逻辑设备一起自动创建
vkGetDeviceQueue(device,indices.graphicsFamily.value(),0,&graphicsqueue);
vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

}

//swapchain
void Application_Device::SGvk_Device_Create_SwapChain()
{
    SwapChainSupportDetails swapChainSupport = Vk_Device_Status_QuerySwapChainSupport(physicalDevice);//需要检查的三种属性1.表面格式（像素格式、色彩空间）
    //2.可用的演示模式

    VkSurfaceFormatKHR surfaceFormat = SGvk_Device_Choose_SwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = SGvk_Device_Choose_SwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = SGvk_Device_Choose_Choose_SwapExtent(swapChainSupport.capabilities);

    //希望交换链拥有多个图像
    //uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
    //至少请求一个比最小值多一个图像
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    //确保在执行此操作时不超过最大图像数
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    //创建交换链对象
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = SGvk_Device_Choose_QueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    //假设图形队列和法向量队列是相同的
    if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0; // Optional
    createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    //指定转换(如果支持)
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    //指定是否应将 Alpha 通道用于 与窗口系统中的其他窗口混合
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    //presentMode和开启裁剪
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;
//创建交换链
if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create swap chain!");
}
    
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

//imageviews
void Application_Device::SGvk_Device_Create_ImageViews()
{
swapChainImageViews.resize(swapChainImages.size());

for (size_t i = 0; i < swapChainImages.size(); i++) 
{
    swapChainImageViews[i] = SGvk_Device_Create_ImageView_AttachFuc(swapChainImages[i], swapChainImageFormat,VK_IMAGE_ASPECT_COLOR_BIT);
}
}
//
VkImageView Application_Device::SGvk_Device_Create_ImageView_AttachFuc(VkImage image, VkFormat format,VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture image view!");
    }

    return imageView;
}

//renderpass
void Application_Device::SGvk_Device_Create_RenderPass()
{
//颜色附件
VkAttachmentDescription colorAttachment{};
colorAttachment.format = swapChainImageFormat;
colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;//采样率

colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//表示在渲染开始时清除附件的内容
colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;//表示在渲染结束后保留附件的内容

colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;//模板加载和存储操作(dont care表示不进行操作)
colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//现有内容未定 我们不关心他()

colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//表示不关心附件的初始布局(设置初始布局)
colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//

//深度附件
VkAttachmentDescription depthAttachment{};
depthAttachment.format = SGvk_Device_Choose_FindDepthFormat();
depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

//子通道和附件参照
VkAttachmentReference colorAttachmentRef{};//颜色附件参考
colorAttachmentRef.attachment = 0;//表示使用第一个附件
colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//表示附件在子通道中的布局为颜色附件布局

//深度
VkAttachmentReference depthAttachmentRef{};
depthAttachmentRef.attachment = 1;
depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

VkSubpassDescription subpass{};//子通道
subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;//表示子通道用于图形管线

subpass.colorAttachmentCount = 1;//颜色附件数量为1
subpass.pColorAttachments = &colorAttachmentRef;//layout(location = 0) out vec4 outColor //指向颜色附件参考的指针
subpass.pDepthStencilAttachment = &depthAttachmentRef;
//attachments
std::array<VkAttachmentDescription,2> attachments = {colorAttachment,depthAttachment};


VkRenderPassCreateInfo renderPassInfo{};
renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());//设置附件数量
renderPassInfo.pAttachments = attachments.data();//指向颜色附件
renderPassInfo.subpassCount = 1;//设置子通道数量
renderPassInfo.pSubpasses = &subpass;//指向子通道

//子通道依赖关系
VkSubpassDependency dependency{};
dependency.srcSubpass = VK_SUBPASS_EXTERNAL;//表示依赖关系的源子通道为外部子通道
dependency.dstSubpass = 0;//表示依赖关系的目标子通道为第一个子通道
dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;//表示依赖关系的源阶段为颜色附件输出阶段。
dependency.srcAccessMask = 0;//表示对源子通道的访问方式为0(设置源访问掩码)
dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;//表示对目标子通道的访问方式为写入颜色附件

renderPassInfo.dependencyCount = 1;
renderPassInfo.pDependencies = &dependency;

if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create render pass!");
}
}

void Application_Device::SGvk_Device_Create_DescriptorSetLayout()
{
VkDescriptorSetLayoutBinding uboLayoutBinding{};
uboLayoutBinding.binding = 0;
uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
uboLayoutBinding.descriptorCount = 1;

uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;//指定在VERTEX阶段使用
uboLayoutBinding.pImmutableSamplers = nullptr;//跟预采样有关

//新描述符
VkDescriptorSetLayoutBinding samplerLayoutBinding{};
samplerLayoutBinding.binding = 1;
samplerLayoutBinding.descriptorCount = 1;
samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
samplerLayoutBinding.pImmutableSamplers = nullptr;
samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;//指定在Fragment阶段使用
//绑定描述符
std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
VkDescriptorSetLayoutCreateInfo layoutInfo{};
layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
layoutInfo.pBindings = bindings.data();


if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create descriptor set layout!");
}
}

void Application_Device::SGvk_Device_Create_GraphicsPipeline(std::string vert,std::string frag)
{
    auto vertShaderCode = readFile(vert.c_str());
    auto fragShaderCode = readFile(frag.c_str());
    std::cout << "vertShaderCode has Filesize : " <<vertShaderCode.size() << std::endl;
    std::cout << "fragShaderCode has Filesize : " <<fragShaderCode.size() << std::endl;
    //创建着色器模块
    VkShaderModule vertShaderMoudle = SGvk_Device_Create_ShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = SGvk_Device_Create_ShaderModule(fragShaderCode);

    //着色器阶段创建
    //vert
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

    vertShaderStageInfo.module = vertShaderMoudle;
    vertShaderStageInfo.pName = "main";

    //frag
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    
    
    //顶点输入
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    
    //输入程序集
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    //指定计数
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

/*如果没有动态状态，则需要使用
VkPipelineViewportStateCreateInfo viewportState{};
viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
viewportState.viewportCount = 1;
viewportState.pViewports = &viewport;
viewportState.scissorCount = 1;
viewportState.pScissors = &scissor;*/

//光栅器 

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;

    rasterizer.rasterizerDiscardEnable = VK_FALSE;

    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;


    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

//多重采样

VkPipelineMultisampleStateCreateInfo multisampling{};
multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
multisampling.sampleShadingEnable = VK_FALSE;
multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
multisampling.minSampleShading = 1.0f; // Optional
multisampling.pSampleMask = nullptr; // Optional
multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
multisampling.alphaToOneEnable = VK_FALSE; // Optional


//深度和模板测试

//颜色混合

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

//动态状态
    std::vector<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,    
    VK_DYNAMIC_STATE_SCISSOR
    };
    
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    

//管道布局
VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
pipelineLayoutInfo.setLayoutCount = 1; // Optional
pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout; // Optional
pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional



if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create pipeline layout!");
}
//深度
VkPipelineDepthStencilStateCreateInfo depthStencil{};
depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
depthStencil.depthTestEnable = VK_TRUE;
depthStencil.depthWriteEnable = VK_TRUE;
depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
depthStencil.depthBoundsTestEnable = VK_FALSE;
depthStencil.stencilTestEnable = VK_FALSE;


VkGraphicsPipelineCreateInfo pipelineInfo{};
pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
pipelineInfo.stageCount = 2;
pipelineInfo.pStages = shaderStages;
//深度
pipelineInfo.pDepthStencilState = &depthStencil;

pipelineInfo.pVertexInputState = &vertexInputInfo;
pipelineInfo.pInputAssemblyState = &inputAssembly;
pipelineInfo.pViewportState = &viewportState;
pipelineInfo.pRasterizationState = &rasterizer;
pipelineInfo.pMultisampleState = &multisampling;

pipelineInfo.pColorBlendState = &colorBlending;
pipelineInfo.pDynamicState = &dynamicState;

pipelineInfo.layout = pipelineLayout;

pipelineInfo.renderPass = renderPass;
pipelineInfo.subpass = 0;

pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
pipelineInfo.basePipelineIndex = -1; // Optional

if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create graphics pipeline!");
}
    //销毁着色器模块
    vkDestroyShaderModule(device,fragShaderModule,nullptr);
    vkDestroyShaderModule(device,vertShaderMoudle,nullptr);

}
//shadermodule
VkShaderModule Application_Device::SGvk_Device_Create_ShaderModule(const std::vector<char> code)
{
    VkShaderModuleCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    VkShaderModule shaderModule;
if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create shader module!");
    }
    return shaderModule;
}

void Application_Device::SGvk_Device_Create_DepthResources()
{
    VkFormat depthFormat = SGvk_Device_Choose_FindDepthFormat();
    SGvk_Device_Create_Image(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = SGvk_Device_Create_ImageView(depthImage,depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

//createImage
void Application_Device::SGvk_Device_Create_Image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create image!");
    }
    //缓冲区分配
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);//获取到图像对象image所需的内存类型索引和内存大小等信息

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, imageMemory, 0);
}

//缓冲区类型分配findMemoryType
uint32_t Application_Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
//查询可用关于内存类型的信息
VkPhysicalDeviceMemoryProperties memProperties;
vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        return i;
    }
}

SG_CORE_ERROR("failed to find suitable memory type!");

}

VkImageView Application_Device::SGvk_Device_Create_ImageView(VkImage image, VkFormat format,VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture image view!");
    }

    return imageView;

}

//create framebuffer
void Application_Device::SGvk_Device_Create_Framebuffers()
{
    swapChainFramebuffers.resize(swapChainImageViews.size());
    //遍历图像创建帧缓冲区
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            std::array<VkImageView, 2> attachments = {
                swapChainImageViews[i],
                depthImageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                SG_CORE_ERROR("failed to create framebuffer!");
            }
        }
}

//pool
void Application_Device::SGvk_Device_Create_CommandPool()
{
QueueFamilyIndices queueFamilyIndices = SGvk_Device_Choose_QueueFamilies(physicalDevice);

VkCommandPoolCreateInfo poolInfo{};
poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;//表示命令池中的命令缓冲可以被重置
poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create command pool!");
}
}
//TextureImage
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Application_Device::SGvk_Device_Create_TextureImage(std::string texture_path)
{
int texWidth,texHeight,texChannels;
stbi_uc* pixels = stbi_load(texture_path.c_str(),&texWidth,&texHeight,&texChannels,STBI_rgb_alpha);

VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        SG_CORE_ERROR("failed to load texture image!");
    }

VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;
SGvk_Device_Create_Buffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

void* data;
vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
memcpy(data, pixels, static_cast<size_t>(imageSize));
vkUnmapMemory(device, stagingBufferMemory);

//设置mip层数
mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth,texHeight)))) + 1;

stbi_image_free(pixels);

SGvk_Device_Create_Image(texWidth, texHeight,VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT
 | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

SGvk_Device_Create_TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

SGvk_Device_Create_TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
 
vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

 
}

void Application_Device::SGvk_Device_Create_TextureImageView()
{
textureImageView = SGvk_Device_Create_ImageView_AttachFuc(textureImage, VK_FORMAT_R8G8B8A8_SRGB,VK_IMAGE_ASPECT_COLOR_BIT);
}

//Buffer
void Application_Device::SGvk_Device_Create_Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer&  buffer, VkDeviceMemory& bufferMemory)
{
VkBufferCreateInfo bufferInfo{};
bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
bufferInfo.size = size;
bufferInfo.usage = usage;
bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create buffer!");
}

VkMemoryRequirements memRequirements;
vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

VkMemoryAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
allocInfo.allocationSize = memRequirements.size;
allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to allocate buffer memory!");
}

vkBindBufferMemory(device, buffer, bufferMemory, 0);

}
//Beginlayout
VkCommandBuffer Application_Device::beginSingleTimeCommands(VkCommandPool cmdPool) {
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
void Application_Device::endSingleTimeCommands(VkCommandBuffer commandBuffer,VkCommandPool cmdPool) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsqueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsqueue);

    vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);
}
//创建布局过度
void Application_Device::SGvk_Device_Create_TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) 
{ 
VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);

VkImageMemoryBarrier barrier{};
barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
barrier.oldLayout = oldLayout;
barrier.newLayout = newLayout;

barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

barrier.image = image;
barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
barrier.subresourceRange.baseMipLevel = 0;
barrier.subresourceRange.levelCount = 1;
barrier.subresourceRange.baseArrayLayer = 0;
barrier.subresourceRange.layerCount = 1;

barrier.srcAccessMask = 0; // TODO
barrier.dstAccessMask = 0; // 

VkPipelineStageFlags sourceStage;
VkPipelineStageFlags destinationStage;

if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
} else {
    throw std::invalid_argument("unsupported layout transition!");
}
//
if(newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL){
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if(hasStencilComponent(format)){
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
} else{
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
}
//添加正确的访问掩码和管道阶段

vkCmdPipelineBarrier(
    commandBuffer,
    sourceStage, destinationStage,
    0,
    0, nullptr,
    0, nullptr,
    1, &barrier
);
endSingleTimeCommands(commandBuffer,commandPool);
}
//将一个 Vulkan 缓冲区（buffer）中的数据复制到一个 Vulkan 图像（image）中
//参数:1.源缓冲区，包含待复制的数据。2.目标图像，数据将被复制到这个图像中 3.图像的宽度 4.图像的高度
void Application_Device::copyBufferToImage(VkBuffer buffer, VkImage image,uint32_t width,uint32_t height)
{
VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);

VkBufferImageCopy region{};
region.bufferOffset = 0;
region.bufferRowLength = 0;
region.bufferImageHeight = 0;

region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
region.imageSubresource.mipLevel = 0;
region.imageSubresource.baseArrayLayer = 0;
region.imageSubresource.layerCount = 1;

region.imageOffset = {0, 0, 0};
region.imageExtent = {
    width,
    height,
    1
};

vkCmdCopyBufferToImage(
    commandBuffer,
    buffer,
    image,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1,
    &region
);
endSingleTimeCommands(commandBuffer,commandPool);
}

//sampler
void Application_Device::SGvk_Device_Create_TextureSampler()
{
VkSamplerCreateInfo samplerInfo{};
samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
samplerInfo.magFilter = VK_FILTER_LINEAR;
samplerInfo.minFilter = VK_FILTER_LINEAR;

samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

VkPhysicalDeviceProperties properties{};
vkGetPhysicalDeviceProperties(physicalDevice, &properties);

//下面两个是否应用各向异性过滤 值越低性能越好
samplerInfo.anisotropyEnable = VK_TRUE;
samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;//设备支持最高值
//该字段指定在采样超出范围时返回的颜色 具有钳位到边框寻址模式的图像。可以返回黑色， 浮动或整数格式的白色或透明。不能指定 任意颜色
samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
//该字段指定所需的坐标系 用于寻址图像中的纹素
samplerInfo.unnormalizedCoordinates = VK_FALSE;

samplerInfo.compareEnable = VK_FALSE;
samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
samplerInfo.mipLodBias = 0.0f;
samplerInfo.minLod = 0.0f;
samplerInfo.maxLod = 0.0f;

 if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
       SG_CORE_ERROR("failed to create texture sampler!");
    }
}
//加载模型
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
//model
std::vector<Vertex> vertices;
std::vector<uint32_t> indices;
void loadModel(std::string Model_Path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, Model_Path.c_str())) {
        throw std::runtime_error(warn + err);
    }
    std::unordered_map<Vertex,uint32_t> uniqueVertices{};

    //将文件中的所有面合并到一个模型中(遍历所有形状)
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};
            vertex.pos = {
            attrib.vertices[3 * index.vertex_index + 0],
            attrib.vertices[3 * index.vertex_index + 1],
            attrib.vertices[3 * index.vertex_index + 2]
        };

            vertex.texCoord = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
        };

            vertex.color = {1.0f, 1.0f, 1.0f};

        if (uniqueVertices.count(vertex) == 0){
            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex);
        }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void Application_Device::SGvk_Device_Create_VertexBuffer()
{
VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
//临时缓冲区
VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;
SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

void* data;
vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
memcpy(data, vertices.data(), (size_t) bufferSize);
vkUnmapMemory(device, stagingBufferMemory);

SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
//清理
vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

}

void Application_Device::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
 {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer,commandPool);
}


void Application_Device::SGvk_Device_Create_IndexBuffer()
{
VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;
SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

void* data;
vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
memcpy(data, indices.data(), (size_t) bufferSize);
vkUnmapMemory(device, stagingBufferMemory);

SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

copyBuffer(stagingBuffer, indexBuffer, bufferSize);

vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

}

void Application_Device::SGvk_Device_Create_UniformBuffers()
{
VkDeviceSize bufferSize = sizeof(UniformBufferObject);

uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
{
    SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    , uniformBuffers[i], uniformBuffersMemory[i]);

    vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
}

}

//DescriptorPool
void Application_Device::SGvk_Device_Create_DescriptorPool()
{

std::array<VkDescriptorPoolSize, 2> poolSizes{};
poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

VkDescriptorPoolCreateInfo poolInfo{};
poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
poolInfo.pPoolSizes = poolSizes.data();
poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create descriptor pool!");
}

}

//DescriptorSets
void Application_Device::SGvk_Device_Create_DescriptorSets()
{
std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
VkDescriptorSetAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
allocInfo.descriptorPool = descriptorPool;
allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
allocInfo.pSetLayouts = layouts.data();

descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to allocate descriptor sets!");
}

for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

VkDescriptorImageInfo imageInfo{};
imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
imageInfo.imageView = textureImageView;
imageInfo.sampler = textureSampler;

std::array<VkWriteDescriptorSet,2> descriptorWrites{};
descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
descriptorWrites[0].dstSet = descriptorSets[i];
descriptorWrites[0].dstBinding = 0;
descriptorWrites[0].dstArrayElement = 0;

descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
descriptorWrites[0].descriptorCount = 1;

descriptorWrites[0].pBufferInfo = &bufferInfo;
descriptorWrites[0].pImageInfo = nullptr; // Optional
descriptorWrites[0].pTexelBufferView = nullptr; // Optional

descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
descriptorWrites[1].dstSet = descriptorSets[i];
descriptorWrites[1].dstBinding = 1;
descriptorWrites[1].dstArrayElement = 0;
descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
descriptorWrites[1].descriptorCount = 1;
descriptorWrites[1].pImageInfo = &imageInfo;

vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

}
}

void Application_Device::SGvk_Device_Create_CommandBuffer()
{
commandBuffers.resize(swapChainImageViews.size());

VkCommandBufferAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
allocInfo.commandPool = commandPool;
allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;//表示命令缓冲为主要级别
allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();//命令缓冲数量

if (vkAllocateCommandBuffers(device, &allocInfo,commandBuffers.data()) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to allocate command buffers!");
}
}

void Application_Device::SGvk_Device_Create_SyncObjects()
{
imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);


VkSemaphoreCreateInfo semaphoreInfo{};
semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

VkFenceCreateInfo fenceInfo{};
fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;//第一次调用立即返回

for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
{
if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
    vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
    vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create semaphores!");
}

}
}

/** Destruction
 *  void recreateSwapChain();
 * 
 * 
 * 
 * 
 * 
*/
void Application_Device::recreateSwapChain()
{
    int width = 0,height = 0;
glfwGetFramebufferSize(wd,&width,&height);
while (width == 0 || height == 0) {
    glfwGetFramebufferSize(wd, &width, &height);
    glfwWaitEvents();
}
vkDeviceWaitIdle(device);

cleanSwapChain();
//Imgui


 // Recreate main application Vulkan resources
SGvk_Device_Create_SwapChain();
SGvk_Device_Create_ImageViews();
SGvk_Device_Create_DepthResources();
SGvk_Device_Create_RenderPass();
SGvk_Device_Create_GraphicsPipeline("../SegoSystem/Renderer/shaders/vert.spv","../SegoSystem/Renderer/shaders/frag.spv");
SGvk_Device_Create_Framebuffers();
SGvk_Device_Create_DescriptorPool();
SGvk_Device_Create_CommandBuffer();
}

void Application_Device::cleanSwapChain()
{
    //清除depth
vkDestroyImageView(device, depthImageView, nullptr);
vkDestroyImage(device, depthImage, nullptr);
vkFreeMemory(device, depthImageMemory, nullptr);
//清除CommandBuffer
vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()),
                         commandBuffers.data());

vkDestroyPipeline(device, graphicsPipeline, nullptr);
vkDestroyRenderPass(device, renderPass, nullptr);


for(size_t i = 0; i < swapChainFramebuffers.size();i++)
{
    vkDestroyFramebuffer(device,swapChainFramebuffers[i],nullptr);
}

for(size_t i = 0; i <swapChainImageViews.size();i++)
{
    vkDestroyImageView(device,swapChainImageViews[i],nullptr);
}

 vkDestroySwapchainKHR(device, swapChain, nullptr);

}

void Application_Device::recordCommandBuffer(VkCommandBuffer commandBuffer,uint32_t imageIndex)
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
renderPassInfo.renderPass = renderPass;
renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];

renderPassInfo.renderArea.offset = {0,0};
renderPassInfo.renderArea.extent = swapChainExtent;
//清除值
std::array<VkClearValue, 2> clearColor = {};
clearColor[0].color = {{0.0f,0.0f,0.0f,1.0f}};
clearColor[1].depthStencil = {1.0f,0};

renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColor.size());
renderPassInfo.pClearValues = clearColor.data();

vkCmdBeginRenderPass(commandBuffer,&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);//开始渲染通道

//绑定图形管道
vkCmdBindPipeline(commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,graphicsPipeline);
//视口与剪刀(可以定为动态状态或者静态状态)
    VkViewport viewport{};
    viewport.x = 0.0f,
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer,0,1,&viewport);
    //我们选择一个完整的剪刀矩形
    VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    
VkBuffer vertexBuffers[] = {vertexBuffer};
VkDeviceSize offsets[] = {0};

//使用索引缓冲区
vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
//绑定描述符集
vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
//vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
//vkCmdDraw(commandBuffer, 3, 1, 0, 0);

vkCmdEndRenderPass(commandBuffer);//结束渲染通风道

if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to record command buffer!");
}
}

void Application_Device::cleanup()
{

    //清理交换链 
    cleanSwapChain();
    //清理texture
    vkDestroySampler(device,textureSampler,nullptr);
    vkDestroyImageView(device,textureImageView,nullptr);
    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);
    //清理均匀缓冲区
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(device,descriptorPool,nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    vkDestroyBuffer(device,indexBuffer,nullptr);
    vkFreeMemory(device, indexBufferMemory , nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);//清理缓冲区
    vkFreeMemory(device, vertexBufferMemory, nullptr);

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

    vkDestroyRenderPass(device, renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);


  
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(wd);
}





