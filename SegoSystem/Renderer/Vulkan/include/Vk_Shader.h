#pragma once
#include <string_view>
#include <vulkan/vulkan.h>

namespace vulkan::resource::Shader
{
    VkShaderModule CreateShaderModule(std::string_view path);
    



    
}