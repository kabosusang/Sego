#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <functional>
#include "Renderer/Vulkan/Vk_Allocate.h"
#include "Renderer/Vulkan/Vk_Create_Resource.h"

#include "Camera/camera.h"

//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------*/


//Data----------------------------------
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"//标准诊断层
};
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME //交换链
};
//-----------------------------------
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = false;
#endif
//创建CreateDebugUtilsMessengerEXT
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    SG_CORE_ERROR("validation layer: {0}",pCallbackData->pMessage);
    return VK_FALSE;
}


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily = std::nullopt;
    std::optional<uint32_t> presentFamily = std::nullopt;

    bool isComplete(){
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

//交换链格式
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

//加载着色器代码
static std::vector<char>readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();//tellg() 用于在输入流中获取位置
    std::vector<char> buffer(fileSize);
    //回到文件开头 并读取所有字节
    file.seekg(0);//seekg()用于设置在输入流中的位置
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}



#define MAX_FRAMES_IN_FLIGHT 2
/*-------------------------------------------------------*/
////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


class Application_Device
{
public:
    //Window
    inline void InputWindow(GLFWwindow* window){ wd = window;}
    void InitVulkan();
    void InitResource();

    //create
    void SGvk_Device_Create_Instance(); //instance
    void setupDebugMessenger(); //mess
    void SGvk_Device_Create_Surface(); //surface
    void SGvk_Device_Create_LogicalDevice();//log
 
    void SGvk_Device_Create_SwapChain(); //swp
    void SGvk_Device_Create_ImageViews();//imv
    VkImageView SGvk_Device_Create_ImageView_AttachFuc(VkImage image, VkFormat format,VkImageAspectFlags aspectFlags);
    void SGvk_Device_Create_RenderPass();//ren
    void SGvk_Device_Create_DescriptorSetLayout();//Layout
    void SGvk_Device_Create_GraphicsPipeline(std::string vert,std::string frag);//pipline
    VkShaderModule SGvk_Device_Create_ShaderModule(const std::vector<char> code);
    void SGvk_Device_Create_DepthResources();//Depth
    //img
    void SGvk_Device_Create_Image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory); //img
    VkImageView SGvk_Device_Create_ImageView(VkImage image, VkFormat format,VkImageAspectFlags aspectFlags);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    //void copyBufferToImage(VkBuffer buffer, VkImage image,uint32_t width,uint32_t height);
    //
    void SGvk_Device_Create_Framebuffers();//Fram
    void SGvk_Device_Create_CommandPool();//pool
    //void SGvk_Device_Create_TextureImage(std::string texture_path);//texture
    //void SGvk_Device_Create_TextureImageView();
    //void SGvk_Device_Create_Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    
    //////////////////////////-------Vertex----------------///////////////////////////////
    //void SGvk_Device_Create_TextureSampler();
    //void SGvk_Device_Create_VertexBuffer();
    //void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    //void SGvk_Device_Create_IndexBuffer();
    /// @ ////////////////////////////////////////////////////////

    void SGvk_Device_Create_UniformBuffers();
    void SGvk_Device_Create_DescriptorPool();
    void SGvk_Device_Create_DescriptorSets(std::vector<SG_Texture>& textures);
    void SGvk_Device_Create_CommandBuffer();
    void SGvk_Device_Create_SyncObjects();


    void SGvk_Device_Choose_Create_PhysicalDevice();
    /***************************************************************************************/
    //choose
    QueueFamilyIndices SGvk_Device_Choose_QueueFamilies(VkPhysicalDevice device);
    VkSurfaceFormatKHR SGvk_Device_Choose_SwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR SGvk_Device_Choose_SwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D SGvk_Device_Choose_Choose_SwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    //choose -- RenderPass
    VkFormat SGvk_Device_Choose_FindDepthFormat();
    VkFormat SGvk_Device_Choose_FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  
    //Status
    bool Vk_Device_Status_PhysicalDevice(VkPhysicalDevice device );
    static bool Vk_Device_Status_CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails Vk_Device_Status_QuerySwapChainSupport(VkPhysicalDevice device);

    
    void recordCommandBuffer(std::vector<SG_Model>& models,VkCommandBuffer commandBuffer,uint32_t imageIndex);

    //Destruction
    void recreateSwapChain();
    void cleanSwapChain();
    void cleanup(std::vector<SG_Model>& models);

    //OUTPUT DATA
    /*************************/
    inline VkDevice& GetDevice() {return device;}
    



public:
    GLFWwindow* wd;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    std::vector<VkImageView> swapChainImageViews; // views
    std::vector<VkImage> swapChainImages; //images
    VkSwapchainKHR swapChain; //交换链
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout; //描述绑定符
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;//图像管道对象

    std::vector<VkFramebuffer> swapChainFramebuffers;//framebuffers

   
    //深度图像和视图
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkCommandPool commandPool;//commandPool
    std::vector<VkCommandBuffer> commandBuffers;//命令缓冲

    //---------------------------------
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;


    //队列
    VkQueue graphicsqueue;//渲染队列
    VkQueue presentQueue;//呈现队列
    
    //均匀缓冲区
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    //描述符池
    VkDescriptorPool descriptorPool;
    //描述符集
    std::vector<VkDescriptorSet> descriptorSets;

    //信号量和fence
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;//帧索引
    bool framebufferResized = false;


    uint32_t image_count = 0;
};















