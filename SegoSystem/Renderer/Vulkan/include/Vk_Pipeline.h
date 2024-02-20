#pragma once
#include <vulkan/vulkan.h>
#include <string_view>

namespace vulkan::resource::Pipeline
{
    void CreateGraphicsPipeline(std::string_view vshader_path,std::string_view fshader_path_,VkVertexInputBindingDescription bindingDescription,
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions);


    
}
