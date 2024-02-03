#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


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

namespace vulkan::resource::image
{

    

    void CreateImage(uint32_t width,uint32_t height,uint32_t depth,
    uint32_t numArray,uint32_t mipMapCount,VkImageType type,bool isCubeMap
    ,VkSampleCountFlagBits numSamples,VkFormat format, VkImageTiling tiling,VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,VkImage& image,VkDeviceMemory& imageMemory);


    void CreateImageView(VkImage image,VkImageViewType type,VkFormat format,VkImageAspectFlags aspectFlags,
    uint32_t mipMapCount,uint32_t numArray,VkImageView& imageview);

    void CreateSampler(VulkanTextureFilterType texFilter,
    VulkanTextureAddressingType texAddressing,VulkanTextureBorderColorType texBorderColor,
    bool enableAnisotropy,float minLod,float maxLod,float mipLodBias,VkSampler&sampler);


    void GenerateMipmaps(VkCommandBuffer cmdBuffer,VkImage image,VkFormat VkImageFormatListCreateInfo,
    int32_t width,int32_t height,uint32_t mipMapCount,uint32_t numArray,bool autoMipMap);

    //Select
    VkFilter TransformVkFilter(VulkanTextureFilterType typeFilter,VulkanTextureFilterSizeType typeFilterSize); //Selcet Filter
    VkSamplerMipmapMode TransformVkSamplerMipmapMpde(VulkanTextureFilterType typeFilter);
    VkSamplerAddressMode TransformVkSamplerAddressMode(VulkanTextureAddressingType addtype);
    VkBorderColor Util_Transform2VkBorderColor(VulkanTextureBorderColorType type);

   


    //Copy
    void copyBufferToImage(VkCommandBuffer cmdBuffer,VkBuffer buffer,VkImage image,
    uint32_t width,uint32_t height,uint32_t depth,uint32_t numArray);


};