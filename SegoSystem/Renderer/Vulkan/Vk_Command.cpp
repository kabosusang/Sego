#include "Renderer/Vulkan/Vk_Command.h"

VkCommandBuffer SG_COMD::beginSingleTimeCommands(VkDevice& device,VkCommandPool& cmdPool)
{

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

void SG_COMD::endSingleTimeCommands(VkQueue& endqueue,VkDevice& device,VkCommandBuffer& commandBuffer,VkCommandPool& cmdPool)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(endqueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(endqueue);

    vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);

}