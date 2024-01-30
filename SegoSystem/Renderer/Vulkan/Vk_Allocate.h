#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <string>
#include "Log/Log.h"
#include "Renderer/Vulkan/Vk_Command.h"

/**
 * SGvk_Device_Create_Buffer
 * 
*/

namespace SG_Allocate
{

//createBuffer
void SGvk_Device_Create_Buffer(VkDeviceSize size, VkBufferUsageFlags usage, 
VkMemoryPropertyFlags properties, VkBuffer&  buffer, 
VkDeviceMemory& bufferMemory,VkDevice& device,
VkPhysicalDevice& physicalDevice);

//find Type
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,VkPhysicalDevice& phys);

//CreateImage
void SGvk_Device_Create_Image(VkPhysicalDevice& phys,VkDevice& device,uint32_t width, 
uint32_t height,uint32_t mipLevels, VkFormat format, 
VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
VkImage& image, VkDeviceMemory& imageMemory);

//Append
void SGvk_Device_Create_TransitionImageLayout(VkQueue& endque,VkDevice& device,
VkCommandPool& commandPool,VkImage& image, VkFormat format, 
VkImageLayout oldLayout, VkImageLayout newLayout,uint32_t mipLevels);

void copyBufferToImage(VkQueue& endque,VkDevice& device,VkCommandPool& cmdPool,VkBuffer buffer, VkImage& image,uint32_t width,uint32_t height);
inline bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }   


void copyBuffer(VkBuffer Buffer, VkBuffer dstBuffer, VkDeviceSize size,VkDevice& device,
VkCommandPool& cmdPool,VkQueue& endque);



}


