#pragma once
#include <vulkan/vulkan.h>

namespace vulkan::console::command
{
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer cmdbuffer);
};
