#include "pch.h"
#include "VK_Global_Data.h"
#include "../include/Vk_Buffer.h"
#include "../include/Vk_Memory.h"

void vulkan::resource::buffer::CreateBuffer(VkDeviceSize size, 
VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
 VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(g_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(g_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = (memRequirements.memoryTypeBits);

    if (vkAllocateMemory(g_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(g_device, buffer, bufferMemory, 0);

}
