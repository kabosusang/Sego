#pragma once
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

enum class texColorformat
{
	TX_ALPHA = 1,
	TX_RGB = 3,
	TX_RGBA = 4
};

enum VulkanTextureFilterType
{
    Vulkan_TextureFilter_None = 0,
    Vulkan_TextureFilter_Bilinear,
    Vulkan_TextureFilter_Trilinear,
    Vulkan_TextureFilter_Anisotropic
};

enum VulkanTextureFilterSizeType
{
    VulkanTextureFilterSize_Min = 0,
    VulkanTextureFilterSize_Mag
};

enum VulkanTextureAddressingType
{
Vulkan_TextureAddressing_Wrap = 0,
Vulkan_TextureAddressing_Mirror,
Vulkan_TextureAddressing_Clamp,
Vulkan_TextureAddressing_Border
};

enum VulkanTextureBorderColorType
{
    Vulkan_TextureBorderColor_OpaqueBlack = 0,          //0: OpaqueBlack
    Vulkan_TextureBorderColor_OpaqueWhite,              //1: OpaqueWhite
    Vulkan_TextureBorderColor_TransparentBlack,         //2: TransparentBlack

    Vulkan_TextureBorderColor_Count,
};

class TextureCommon
{
public:
~TextureCommon();
static TextureCommon* Create(uint16_t Width,uint16_t height);
static TextureCommon* LoadFromFile(std::string& image_file_path);

//Data Init
void LoadImageData(std::string_view Image_path,TextureCommon* texture);
void CreateImage();
void CreateImageView();

//Select
VkFilter TransformVkFilter(VulkanTextureFilterType typeFilter,VulkanTextureFilterSizeType typeFilterSize); //Selcet Filter
VkSamplerMipmapMode TransformVkSamplerMipmapMpde(VulkanTextureFilterType typeFilter);
VkSamplerAddressMode TransformVkSamplerAddressMode(VulkanTextureAddressingType addtype);
VkBorderColor Util_Transform2VkBorderColor(VulkanTextureBorderColorType type);


void CreateImageSampler();

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





