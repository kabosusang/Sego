#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulkan::resource::buffer
{
    void CreateBuffer(VkDeviceSize size,VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,VkBuffer& buffer,VkDeviceMemory& bufferMemory);

    void CopyBuffer(VkBuffer srcBuffer,VkBuffer dstBuffer,VkDeviceSize size);

}