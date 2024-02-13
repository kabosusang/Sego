#pragma once
#include <string_view>
#include <vulkan/vulkan.h>


namespace vulkan::texture
{
    //重载上层texture
    bool CreateTexture1D_Overload_Set4(std::string_view pathAsset_Tex,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory);
    bool CreateTexture2D_Overload_Set4(std::string_view pathAsset_Tex,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory);
    bool CreateTexture2DArray_Overload_Set4(const std::vector<std::string> &pathAsset_Texs,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory);
    bool CreateTextureCubeMap_Overload_Set4(const std::vector<std::string> &pathAsset_Texs,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory);


    //Middle Create Texture
    bool CreateTexture2D(std::string_view pathAsset_Tex,
    VkImageType type,VkSampleCountFlagBits numSamples,VkFormat format,
    bool autoMipmap,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory);

    bool CreateTexture2DArray(const std::vector<std::string>& pathAsset_Tex,
    VkImageType type,VkSampleCountFlagBits numSamples,VkFormat format,
    bool autoMipmap,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory);

    bool CreateTexture3D(VkFormat format,const uint8_t* pDataRGBA,
    uint32_t size,uint32_t width,uint32_t height,uint32_t depth,
    VkImage& image,VkDeviceMemory& imageMemory);

    bool CreateTextureCube(const std::vector<std::string>& pathAsset_Tex,
    VkSampleCountFlagBits numSamples,VkFormat format,
    bool autoMipmap,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory);
    


    //Load Data Texture
    bool CreateTexture2D(std::string_view pathAsset_Tex,
    VkImageType type,VkSampleCountFlagBits numSamples,VkFormat format,
    bool autoMipmap,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory,VkBuffer& buffer,
    VkDeviceMemory& bufferMemory);

    bool CreateTexture2DArray(const std::vector<std::string>& pathAsset_Tex,
    VkImageType type,VkSampleCountFlagBits numSamples,VkFormat format,
    bool autoMipmap,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory,VkBuffer& buffer,
    VkDeviceMemory& bufferMemory);

    bool CreateTexture3D(VkFormat format,const uint8_t* pDataRGBA,
    uint32_t size,uint32_t width,uint32_t height,uint32_t depth,
    VkImage& image,VkDeviceMemory& imageMemory,VkBuffer& buffer,
    VkDeviceMemory& bufferMemory);

    bool CreateTextureCube(const std::vector<std::string>& pathAsset_Tex,
    VkSampleCountFlagBits numSamples,VkFormat format,
    bool autoMipmap,uint32_t& mipMapCount,
    VkImage& image,VkDeviceMemory& imageMemory,VkBuffer& buffer,
    VkDeviceMemory& bufferMemory);



};