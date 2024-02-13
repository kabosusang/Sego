#pragma once
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Uniform
{
//Uniform 
std::vector<VkBuffer> uniformBuffers_;
std::vector<VkDeviceMemory> uniformBuffersMemory_;
std::vector<void*> uniformBuffersMapped_;
};