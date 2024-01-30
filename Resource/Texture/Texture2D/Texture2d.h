#pragma once
#include "Texture/texture.h"
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Texture2D
{
public:
~Texture2D();
static Texture2D* Create(uint16_t Width,uint16_t height);
static Texture2D* LoadFromFile(std::string& image_file_path);
public:
    int wdith_;
    int height_;
    texformat texture_format_;

    //Texture Attribute
    uint32_t mipLevels;//mipmaplevel
    uint32_t layerCount; //用于天空盒
    

    //VkMemory
    VkImage textureImage;
    VkDescriptorImageInfo descriptor;
    VkDeviceMemory textureImageMemory;
    //纹理图像视图
    VkImageView textureImageView;
    VkSampler textureSampler;//采样器
};