#include "../include/Vk_Console.h"
#include "Vk_Console.h"

#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "VK_Global_Data.h"
VkCommandBuffer vulkan::console::command::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = app_device->commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer = {};
    vkAllocateCommandBuffers(g_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void vulkan::console::command::EndSingleTimeCommands(VkCommandBuffer cmdbuffer)
{
    vkEndCommandBuffer(cmdbuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdbuffer;

    vkQueueSubmit(app_device->graphicsqueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(app_device->graphicsqueue);
    vkFreeCommandBuffers(g_device, app_device->commandPool, 1, &cmdbuffer);

}
