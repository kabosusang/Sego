#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer/Vulkan/Vk_Allocate.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"


class Sg_UI
{
public:
void SgUI_Input(GLFWwindow* wd,VkInstance& ins,VkSurfaceKHR& sur,VkPhysicalDevice& phys,VkDevice& dev,VkFormat& swa,
std::vector<VkImage>& swap,std::vector<VkImageView>& swapChs,VkExtent2D& swapExtent,VkQueue& grap,VkQueue& pre)
{
window = wd;
instance = ins;
surface = sur;
physicalDevice = phys;
device = dev; 
swapChainImageFormat = swa;
swapChainImages = swap;
swapChainImageViews = swapChs;
swapChainExtent = swapExtent;
graphicsqueue = grap;
presentQueue = pre;

}
void Init_Sg_Imgui();

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
void createUIDescriptorPool();
void createUIRenderPass();
void createUICommandPool(VkCommandPool *cmdPool, VkCommandPoolCreateFlags flags);
void createUICommandBuffers();
void createUIFramebuffers();
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
VkCommandBuffer beginSingleTimeCommands(VkCommandPool cmdPool);
void endSingleTimeCommands(VkCommandBuffer commandBuffer,VkCommandPool cmdPool);

void recordUICommands(uint32_t bufferIdx);


//clean
void cleanupUIResources();
void cleanup();

void UpdataUiCleanDtata(VkFormat& swapformat,std::vector<VkImage>& swapimage,
std::vector<VkImageView>& swapimviews,VkExtent2D Extent
);


public:
//input Device
GLFWwindow* window;
VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkDevice device;
VkFormat swapChainImageFormat;
std::vector<VkImage> swapChainImages;
std::vector<VkImageView> swapChainImageViews;
VkExtent2D swapChainExtent;

 VkQueue graphicsqueue;//
 VkQueue presentQueue;//呈现队列

//Ui Data
VkDescriptorPool uiDescriptorPool;
VkRenderPass uiRenderPass;
VkCommandPool uiCommandPool;
std::vector<VkCommandBuffer> uiCommandBuffers;
std::vector<VkFramebuffer> uiFramebuffers;
uint32_t MinImageCount = 0;
bool showDemoWindow = true;
QueueFamilyIndices queueIndices;


};
