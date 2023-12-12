#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <string>
#include "Log/Log.h"



namespace SG_COMD
{
    VkCommandBuffer beginSingleTimeCommands(VkDevice& device,VkCommandPool& cmdPool);
    void endSingleTimeCommands(VkQueue& endqueue,VkDevice& device,VkCommandBuffer& commandBuffer,VkCommandPool& cmdPool);






} // namespace SG_COM
