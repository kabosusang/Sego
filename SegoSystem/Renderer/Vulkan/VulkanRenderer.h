#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <optional>
#include <set>
#include <map>
//updateUniformBuffer函数包含头文件
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//input imgui
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

static float calculateFPS()
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    static int frameCount = 0;

    if (frameCount == 0)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    frameCount++;

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    if (duration >= 1)
    {
        float fps = static_cast<float>(frameCount) / duration;
        frameCount = 0;
        startTime = endTime;
        return fps;
    }

    return -1.0f; // 返回-1表示FPS还未计算完成
}
//顶点数据

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <array>
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    //绑定说明
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }
    //属性说明
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        return attributeDescriptions;
    }

bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};
namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}


/*
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};*/

//定义UBO
struct UniformBufferObject{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

//-----------------------------------------------//
const int MAX_FRAMES_IN_FLIGHT = 2;//定义多少帧同时处理

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;



const std::string MODEL_PATH = "../SegoSystem/Renderer/models/viking_room.obj";
const std::string TEXTURE_PATH = "../SegoSystem/Renderer/models/viking_room.png";


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"//标准诊断层
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME //交换链
};


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

//队列索引
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

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

#include "Events/Event.h"
#include "VKimgui.h"
#include "Events/WindowEvent.h"
class HelloTriangleApplication 
{
private:
bool m_Runing = true;
public:
void OnEvent(Event& e);
bool OnWindowClose(WindowCloseEvent& e);

void drawUI();
void Imgui_Init();
void createUIDescriptorPool();
void createUIRenderPass();
void createUICommandPool(VkCommandPool *cmdPool, VkCommandPoolCreateFlags flags);
void createUICommandBuffers();
void createUIFramebuffers();
void recordUICommands(uint32_t bufferIdx);
void cleanupUIResources();
//imgui
private:
VkDescriptorPool uiDescriptorPool;
VkRenderPass uiRenderPass;
VkCommandPool uiCommandPool;
std::vector<VkCommandBuffer> uiCommandBuffers;
std::vector<VkFramebuffer> uiFramebuffers;
uint32_t MinImageCount = 0;
bool showDemoWindow = true;
QueueFamilyIndices queueIndices;

private:
    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device; //逻辑设备
    VkQueue graphicsqueue;//
    VkQueue presentQueue;//呈现队列

    VkSurfaceKHR surface; //窗口表面

    VkSwapchainKHR swapChain; //交换链
    std::vector<VkImage> swapChainImages; //
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;

    VkRenderPass renderPass;
    //VkPipelineLayout pipelineLayout;//管道布局layout
    VkPipeline graphicsPipeline;//图像管道对象

    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;//命令缓冲

    //信号量和fence
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;//帧索引
    bool framebufferResized = false;
    //顶点缓冲区
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    //索引缓冲区
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    //描述绑定符
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    //均匀缓冲区
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    //描述符池
    VkDescriptorPool descriptorPool;
    //描述符集
    std::vector<VkDescriptorSet> descriptorSets;
    //Texture
    uint32_t mipLevels;//mipmaplevel
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    //纹理图像视图
    VkImageView textureImageView;
    VkSampler textureSampler;//采样器
    //深度图像和视图
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

public: 
inline bool InputWindow(GLFWwindow* wd) { window = wd; return true; }

void run();

private:
void initWindow();
//窗口大小回调函数
inline static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));

app->framebufferResized = true;
}


void initVulkan(); //初始化Vulkan

void mainloop();//主循环


//重建交换链
void recreateSwapChain();
//清理旧交换链版本
void cleanSwapChain();
//清理
void cleanup();
//渲染主循环
void drawFrame();
//更新统一数据
void updateUniformBuffer(uint32_t currentImage);
//寻找队列
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
//
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) ;
//设置DebugMessenger
void setupDebugMessenger() ;

//创建Vulkan实例
void cretaeInstance();
//检查列表是否存在Layer
bool checkValidationLayerSupport() ;
//检查是否支持交换链
bool checkDeviceExtensionSupport(VkPhysicalDevice device) ;
//返回基于是否启用验证层
std::vector<const char*> getRequiredExtensions();
//回调函数
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
//选择物理设备
void pickPhysicalDevice();
//检查物理设备是否适合我们想要执行的操作
bool isDeviceSuitable(VkPhysicalDevice device);
//创建逻辑设备
void createLogicalDevice();
//创建表面
void createSurface();
//创建交换链
void createSwapChain();
//交换链详情
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) ;
//选出最佳SurfaceFormat
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) ;
//选出最佳presentMode
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) ;
//选出最佳交换范围
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) ;
//图像视图
void createImageViews();
//渲染通道
void createRenderPass();
//提供描述符绑定的详细信息
void createDescriptorSetLayout();
//创建pipeline
void createGraphicsPipeline();
//创建着色器模块
VkShaderModule createShaderModule(const std::vector<char> code);
//创建帧缓冲
void createFramebuffers();
//imageview抽象函数
VkImageView createImageView(VkImage image, VkFormat format,VkImageAspectFlags aspectFlags);
//纹理视图
void createTextureImageView();
//采样器
void createTextureSampler();
//加载图像
void createTextureImage();
//将一个 Vulkan 缓冲区（buffer）中的数据复制到一个 Vulkan 图像（image）中
//参数:1.源缓冲区，包含待复制的数据。2.目标图像，数据将被复制到这个图像中 3.图像的宽度 4.图像的高度
void copyBufferToImage(VkBuffer buffer, VkImage image,uint32_t width,uint32_t height);
//创建布局过度
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) ;
//layout过度
VkCommandBuffer beginSingleTimeCommands(VkCommandPool cmdPool);
//endSingleTimeCommands
void endSingleTimeCommands(VkCommandBuffer commandBuffer,VkCommandPool cmdPool) ;

//createImage
void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) ;
//加载模型
void loadModel();
//创建顶点缓冲区
void createVertexBuffer();
//创建索引缓冲区
void createIndexBuffer();

//抽象缓冲区
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) ;

//从一个缓冲区复制到另一个缓冲区(内存传输操作使用命令缓冲区执行)
//copy
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) ;
//缓冲区类型分配
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) ;

//创建命令池
void createCommandPool();
//创建均匀缓冲区
void createUniformBuffers();
//创建描述符池
void createDescriptorPool();
//创建图像深度
void createDepthResources();
//检查给定的 Vulkan 格式（format）是否包含模板缓冲（stencil buffer）
bool hasStencilComponent(VkFormat format) ;
//辅助选择具有深度的组件格式
VkFormat findDepthFormat() ;
//选择格式
VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) ;
//分配描述符集
void createDescriptorSets();
//命令缓冲buffer
void createCommandBuffer();

//命令缓冲区记录
void recordCommandBuffer(VkCommandBuffer commandBuffer,uint32_t imageIndex);
//创建异步通信
void createSyncObjects();

//class尾部
};

