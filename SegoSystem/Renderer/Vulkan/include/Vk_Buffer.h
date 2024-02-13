#pragma once
#include <vulkan/vulkan.h>

namespace vulkan::resource::buffer
{
    void CreateBuffer(VkDeviceSize size,VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,VkBuffer& buffer,VkDeviceMemory& bufferMemory);

    void CopyBuffer(VkBuffer srcBuffer,VkBuffer dstBuffer,VkDeviceSize size);

}