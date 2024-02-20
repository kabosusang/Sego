
#include "pch.h"
#include "../include/Vk_Shader.h"
#include "VK_Global_Data.h"

VkShaderModule vulkan::resource::Shader::CreateShaderModule(std::string_view path)
{
   auto code = [&](const char* Codepath) -> std::vector<char>
   {
        std::ifstream file(Codepath, std::ios::ate | std::ios::binary);
        
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();//tellg() 用于在输入流中获取位置
        std::vector<char> buffer(fileSize);
        //回到文件开头 并读取所有字节
        file.seekg(0);//seekg()用于设置在输入流中的位置
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
   };

    auto code_ = code(path.data());


    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code_.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code_.data());
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(g_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create shader module!");
    }
    return shaderModule;
}
