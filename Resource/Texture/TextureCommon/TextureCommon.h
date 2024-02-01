#pragma once
#include <iostream>
#include "Vk_image.h"


enum class texColorformat
{
	TX_ALPHA = 1,
	TX_RGB = 3,
	TX_RGBA = 4
};

class TextureCommon
{
public:
~TextureCommon();
static TextureCommon* LoadFromFile(std::string& image_file_path);

//Data Init
void LoadImageData(std::string_view Image_path,TextureCommon* texture);

public:
    //Imaage Attribute
    int width_;
    int height_;
    texColorformat texture_format_;
    //**********VulkanData****************
    /*------------------------------IMAGE----------------------------------------*/
    //Init Image Data
    uint32_t depth_;
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





