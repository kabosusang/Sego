#pragma once
#include <iostream>
#include "Vk_image.h"
#include <string>
#include <vector>

enum class texColorformat
{
	TX_ALPHA = 1,
	TX_RGB = 3,
	TX_RGBA = 4
};

/*
Vulkan_Texture_2D
Vulkan_Texture_1D
Vulkan_Texture_2DArray
Vulkan_Texture_3D
Vulkan_Texture_CubeMap
*/


class TextureCommon
{
public:
~TextureCommon();
static TextureCommon* LoadFromFile(std::vector<std::string>& image_file_path);
void LoadTexture(std::vector<std::string>& image_file_path,int width,int height,int depth);


public:
    //Imaage Attribute
    int width_;
    int height_;
    uint32_t depth_;
    const uint8_t* pDataRGBA;
    texColorformat texture_format_;
    std::string Textype = "Vulkan_Texture_2D";
    //**********VulkanData****************
    /*------------------------------IMAGE----------------------------------------*/
    //Init Image Data
    uint32_t numArray;
    uint32_t mipLevels;//mipmaplevel
    VkImageType type = VK_IMAGE_TYPE_2D;
    bool isCubeMap; 
    VkSampleCountFlagBits numSamples;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkMemoryPropertyFlags properties;
    //Vulkan Data
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    /*------------------------------IMAGEVIEW----------------------------------------*/
    //Init Imageview Data
    VkImageViewType viewtype;
    VkFormat format;
    VkImageAspectFlags aspectFlags;
    VkImageView textureImageView;

    /*------------------------------IMAGESAMPLER----------------------------------------*/
    VulkanTextureFilterType texfilter;
    VulkanTextureAddressingType texAddressing;
    VulkanTextureBorderColorType texBorderColor;
    //是否开启各项异性过滤
    bool enableAnisotropy;
    float minLod;
    float maxLod;
    float mipLodBias;
    //ImageView
    VkSampler textureSampler;//采样器
    //Image
    VkDescriptorImageInfo descriptor;
    
};





