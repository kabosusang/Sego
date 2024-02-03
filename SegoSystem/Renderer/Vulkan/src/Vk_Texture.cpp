#include "pch.h"
#include <stb_image.h>
#include "VK_Global_Data.h"

#include "../include/Vk_Texture.h"
#include "../include/Vk_Buffer.h"
#include "../include/Vk_Image.h"
#include "../include/Vk_Memory.h"
#include "../include/Vk_Console.h"
#include "Vk_Texture.h"

bool vulkan::texture::CreateTexture2D_Overload_Set4(std::string_view pathAsset_Tex, 
uint32_t &mipMapCount, VkImage &image, VkDeviceMemory &imageMemory)
{
    VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
    return CreateTexture2D(pathAsset_Tex,
                               VK_IMAGE_TYPE_2D,
                               VK_SAMPLE_COUNT_1_BIT,
                               typeFormat,
                               true,
                               mipMapCount,
                               image,
                               imageMemory);

}

bool vulkan::texture::CreateTexture2DArray_Overload_Set4(
   const std::vector<std::string> &pathAsset_Texs, uint32_t &mipMapCount,
    VkImage &image, VkDeviceMemory &imageMemory)
{
    VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
    return CreateTexture2DArray(pathAsset_Texs,
                               VK_IMAGE_TYPE_2D,
                               VK_SAMPLE_COUNT_1_BIT,
                               typeFormat,
                               true,
                               mipMapCount,
                               image,
                               imageMemory);
}

bool vulkan::texture::CreateTextureCubeMap_Overload_Set4(
    const std::vector<std::string> &pathAsset_Texs, uint32_t &mipMapCount, 
    VkImage &image, VkDeviceMemory &imageMemory)
{
   VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
    return CreateTextureCube    (   pathAsset_Texs, 
                                    VK_SAMPLE_COUNT_1_BIT, 
                                    typeFormat,
                                    true,
                                    mipMapCount,
                                    image, 
                                    imageMemory);

}

bool vulkan::texture::CreateTexture1D_Overload_Set4(std::string_view pathAsset_Tex, 
uint32_t &mipMapCount, VkImage &image, VkDeviceMemory &imageMemory)
{
    VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
    return CreateTexture2D(pathAsset_Tex,
                               VK_IMAGE_TYPE_1D,
                               VK_SAMPLE_COUNT_1_BIT,
                               typeFormat,
                               true,
                               mipMapCount,
                               image,
                               imageMemory);
}

bool vulkan::texture::CreateTexture2D(std::string_view pathAsset_Tex,
                                      VkImageType type, VkSampleCountFlagBits numSamples, VkFormat format,
                                      bool autoMipmap, uint32_t &mipMapCount, VkImage &image,
                                      VkDeviceMemory &imageMemory)
{
    VkBuffer vkStagingBuffer;
    VkDeviceMemory vkStagingBufferMemory;
    bool result = CreateTexture2D(pathAsset_Tex, 
                                type, 
                                numSamples,
                                format,
                                autoMipmap,
                                mipMapCount,
                                image, 
                                imageMemory, 
                                vkStagingBuffer, 
                                vkStagingBufferMemory);

    vkDestroyBuffer(g_device, vkStagingBuffer, nullptr);
    vkFreeMemory(g_device, vkStagingBufferMemory, nullptr);
    
    return result;
}

bool vulkan::texture::CreateTexture2DArray(
    const std::vector<std::string> &pathAsset_Tex, VkImageType type, 
    VkSampleCountFlagBits numSamples, VkFormat format, bool autoMipmap, 
    uint32_t &mipMapCount, VkImage &image, VkDeviceMemory &imageMemory)
{
    VkBuffer vkStagingBuffer;
    VkDeviceMemory vkStagingBufferMemory;
    bool result = CreateTexture2DArray(pathAsset_Tex, 
                                type, 
                                numSamples,
                                format,
                                autoMipmap,
                                mipMapCount,
                                image, 
                                imageMemory, 
                                vkStagingBuffer, 
                                vkStagingBufferMemory);

    vkDestroyBuffer(g_device, vkStagingBuffer, nullptr);
    vkFreeMemory(g_device, vkStagingBufferMemory, nullptr);
    
    return result;
}

bool vulkan::texture::CreateTexture3D(VkFormat format, 
const uint8_t *pDataRGBA, uint32_t size, uint32_t width, 
uint32_t height, uint32_t depth, VkImage &image, VkDeviceMemory &imageMemory)
{
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool result= CreateTexture3D(format, 
                                    pDataRGBA, 
                                    width,
                                    height,
                                    depth,
                                    size,
                                    image, 
                                    imageMemory, 
                                    vkStagingBuffer, 
                                    vkStagingBufferMemory);

    vkDestroyBuffer(g_device, vkStagingBuffer, nullptr);
    vkFreeMemory(g_device, vkStagingBufferMemory, nullptr);
    return result;
}

bool vulkan::texture::CreateTextureCube(
const std::vector<std::string> &pathAsset_Tex, 
VkSampleCountFlagBits numSamples, VkFormat format, bool autoMipmap, uint32_t &mipMapCount, 
VkImage &image, VkDeviceMemory &imageMemory)
{
    VkBuffer vkStagingBuffer;
    VkDeviceMemory vkStagingBufferMemory;
    bool result= CreateTextureCube( 
                                    pathAsset_Tex,
                                    numSamples, 
                                    format, 
                                    autoMipmap,
                                    mipMapCount,
                                    image,
                                    imageMemory,
                                    vkStagingBuffer, 
                                    vkStagingBufferMemory
                                    );
                                    

    vkDestroyBuffer(g_device, vkStagingBuffer, nullptr);
    vkFreeMemory(g_device, vkStagingBufferMemory, nullptr);
    return result;
}

//Load Data Texture
bool vulkan::texture::CreateTexture2D(std::string_view pathAsset_Tex,
                                      VkImageType type, VkSampleCountFlagBits numSamples, VkFormat format,
                                      bool autoMipmap, uint32_t &mipMapCount, VkImage &image,
                                      VkDeviceMemory &imageMemory, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    //1. Load image data from file
    int width,height,channels;

    stbi_uc* pixels = stbi_load(pathAsset_Tex.data(),
    &width,&height,&channels,STBI_rgb_alpha);

    VkDeviceSize imageSize = width * height * 4;
    mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width,height)))) + 1;

    if(!pixels)
    {
        SG_CORE_ERROR("failed to load texture image!");
        return false;
    }

    //2. Create staging buffer and copy image data to it
    vulkan::resource::buffer::CreateBuffer(imageSize,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,buffer,bufferMemory);

    void* data;
    vkMapMemory(g_device,bufferMemory,0,imageSize,0,&data);
        memcpy(data,pixels,static_cast<size_t>(imageSize));
    vkUnmapMemory(g_device,bufferMemory);

    stbi_image_free(pixels);

    uint32_t depth = 1;
    uint32_t numArray = 1;
    
    //3. Create image
    vulkan::resource::image::CreateImage(width,
    height,
    depth,
    numArray,
    mipMapCount,
    type,
    false,
    numSamples,
    format,
    VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
    VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    image,imageMemory);

    //4. Transition image layout and copy staging buffer to it
    VkCommandBuffer cmdBuffer = vulkan::console::command::BeginSingleTimeCommands();
    {
            vulkan::memory::transitionImageLayout(cmdBuffer,image,
            VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            0,1,0,numArray);
        {
            vulkan::resource::image::copyBufferToImage(cmdBuffer,buffer,image,
            static_cast<uint32_t>(width),static_cast<uint32_t>(height),
            static_cast<uint32_t>(depth),numArray);
        }
            vulkan::memory::transitionImageLayout(cmdBuffer,image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            0,1,0,numArray);

            vulkan::resource::image::GenerateMipmaps(cmdBuffer,image,format,
            static_cast<int32_t>(width),static_cast<int32_t>(height),
            mipMapCount,numArray,autoMipmap);

    }
    vulkan::console::command::EndSingleTimeCommands(cmdBuffer);
    return true;
}


//创建2D纹理数组，注意所有纹理宽高需要相同
bool vulkan::texture::CreateTexture2DArray(
    const std::vector<std::string>& apathAsset_Tex, 
    VkImageType type, VkSampleCountFlagBits numSamples,
    VkFormat format, bool autoMipmap, uint32_t &mipMapCount, 
    VkImage &image, VkDeviceMemory &imageMemory, VkBuffer &buffer, 
    VkDeviceMemory &bufferMemory)
{
    //1. Load image data from file
    std::vector<int> aWidth;
    std::vector<int> aHeight;
    std::vector<stbi_uc*> aPixels;
    size_t Count_tex = apathAsset_Tex.size();
    if(Count_tex <= 0)
    {
        SG_CORE_ERROR("failed to load texture image!");
        return false;
      
    }
    for (size_t i =0 ; i < Count_tex; i++)
    {
        int width,height,channels;
        stbi_uc* pixels = stbi_load(apathAsset_Tex[i].data(),
        &width,&height,&channels,STBI_rgb_alpha);
        if(!pixels)
        {
            SG_CORE_ERROR("failed to load texture image!");
             return false;
        }
        aWidth.push_back(width);
        aHeight.push_back(height);
        aPixels.push_back(pixels);
    }
    int width = aWidth[0];
    int height = aHeight[0];
    for(size_t i = 1; i < Count_tex; i++)
    {
        if(width != aWidth[i] || height != aHeight[i])
        {
            SG_CORE_ERROR("failed to load texture image!");
            return false;
        }

    }
    uint32_t depth = 1;
    uint32_t numArray = static_cast<uint32_t>(Count_tex);
    if(type == VK_IMAGE_TYPE_1D)
    {
        depth = 0;
    }
    //2. Create staging buffer and copy image data to it
    mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width,height)))) + 1;
    VkDeviceSize imageSize = width * height * 4 ;
    VkDeviceSize imageSizeAll = imageSize * Count_tex;

    vulkan::resource::buffer::CreateBuffer(imageSizeAll,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,buffer,bufferMemory);

    for(size_t i = 0; i < Count_tex; i++)
    {
        stbi_uc* pixels = aPixels[i];

        void* data;
        vkMapMemory(g_device,bufferMemory,imageSize * i,imageSize,0,&data);
            memcpy(data,aPixels[i],static_cast<size_t>(imageSize));
        vkUnmapMemory(g_device,bufferMemory);
        stbi_image_free(pixels);
    }
    //3. Create image
    vulkan::resource::image::CreateImage(width,height,depth,numArray,
    mipMapCount,type,false,numSamples,format,VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
    VK_IMAGE_USAGE_SAMPLED_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    image,imageMemory);

    //4. Transition image layout and copy staging buffer to it
    VkCommandBuffer cmbBuffer = vulkan::console::command::BeginSingleTimeCommands();
    {
        vulkan::memory::transitionImageLayout(cmbBuffer,image,
        VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        0,1,0,numArray);
        {
            vulkan::resource::image::copyBufferToImage(cmbBuffer,buffer,image,
            static_cast<uint32_t>(width),static_cast<uint32_t>(height),
            static_cast<uint32_t>(depth),numArray);
        }
        vulkan::memory::transitionImageLayout(cmbBuffer,image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        0,numArray,0,numArray);

        vulkan::resource::image::GenerateMipmaps(cmbBuffer,image,format,
        static_cast<int32_t>(width),static_cast<int32_t>(height),
        mipMapCount,numArray,autoMipmap);
    }

    vulkan::console::command::EndSingleTimeCommands(cmbBuffer);
     return true;
}

bool vulkan::texture::CreateTexture3D(VkFormat format, 
const uint8_t *pDataRGBA, uint32_t size, uint32_t width, 
uint32_t height, uint32_t depth, VkImage &image, 
VkDeviceMemory &imageMemory, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(g_physicalDevice,format,&formatProperties);
    if(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT))
    {
        SG_CORE_ERROR("texture image format does not support sampled image!");
        return false;
    }
    
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(g_physicalDevice, &deviceProperties);
    uint32_t maxImageDimension3D = deviceProperties.limits.maxImageDimension3D;

    if(width > maxImageDimension3D || height > maxImageDimension3D || depth > maxImageDimension3D)
    {
        SG_CORE_ERROR("texture image size is too large!");
        return false;
    }

    //1> Create staging buffer and copy image data to it
    VkDeviceSize imageSize = size;
    vulkan::resource::buffer::CreateBuffer(imageSize,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,buffer,bufferMemory);
    
        
    void* data;
    vkMapMemory(g_device,bufferMemory,0,imageSize,0,&data);
        memcpy(data,pDataRGBA,static_cast<size_t>(imageSize));
    vkUnmapMemory(g_device,bufferMemory);

    //2> Create image
    vulkan::resource::image::CreateImage(width,height,depth,1,1,
    VK_IMAGE_TYPE_3D,false,VK_SAMPLE_COUNT_1_BIT,format,VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
    VK_IMAGE_USAGE_SAMPLED_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    image,imageMemory);

    //3 > Transition image layout and copy staging buffer to it
    VkCommandBuffer cmdBuffer = vulkan::console::command::BeginSingleTimeCommands();
    {
        vulkan::memory::transitionImageLayout(cmdBuffer,image,
        VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        0,1,0,1);
        {
            vulkan::resource::image::copyBufferToImage(cmdBuffer,buffer,image,
            static_cast<uint32_t>(width),static_cast<uint32_t>(height),
            static_cast<uint32_t>(depth),1);
        }
        vulkan::memory::transitionImageLayout(cmdBuffer,image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        0,1,0,1);

        vulkan::resource::image::GenerateMipmaps(cmdBuffer,image,format,
        static_cast<int32_t>(width),static_cast<int32_t>(height),
        1,1,false);
    }
    vulkan::console::command::EndSingleTimeCommands(cmdBuffer);
    return true;
}


//创建Cube Map  6张图片 按X+，X-，Y+，Y-，Z+，Z-顺序加载6张纹理
bool vulkan::texture::CreateTextureCube(const std::vector<std::string> &pathAsset_Tex, 
VkSampleCountFlagBits numSamples,
VkFormat format, bool autoMipmap, uint32_t &mipMapCount, 
VkImage &image, VkDeviceMemory &imageMemory, 
VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    //1. Load image data from file
    std::vector<int> aWidth;
    std::vector<int> aHeight;
    std::vector<stbi_uc*> aPixels;
    size_t Count_tex = pathAsset_Tex.size();
    if(Count_tex <= 0)
    {
        SG_CORE_ERROR("vulkan::texture::CreateTextureCube: Count_tex <= 0");
        return false;
    }
    if(Count_tex !=6)
    {
        SG_CORE_ERROR("vulkan::texture::CreateTextureCube: Count_tex !=6");
        return false;
    }

    for(size_t i = 0; i < Count_tex; i++)
    {
        int width,height,channels;
        stbi_uc* pixels = stbi_load(pathAsset_Tex[i].data(),
        &width,&height,&channels,STBI_rgb_alpha);
        if(!pixels)
        {
            SG_CORE_ERROR("failed to load texture image!");
            return false;
        }
        aWidth.push_back(width);
        aHeight.push_back(height);
        aPixels.push_back(pixels);
    }

    int width = aWidth[0];
    int height = aHeight[0];
    for(size_t i = 1; i < Count_tex; i++)
    {
        if(width != aWidth[i] || height != aHeight[i])
        {
            SG_CORE_ERROR("vulkan::texture::CreateTextureCube: Width Height !=!");
            return false;
        }
    }

    uint32_t depth = 1;
    uint32_t numArray = Count_tex;

    //2. Create staging buffer and copy image data to it
    mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width,height)))) + 1;
    VkDeviceSize imageSize = width * height * 4 ;
    VkDeviceSize imageSizeAll = imageSize * Count_tex;

    vulkan::resource::buffer::CreateBuffer(imageSizeAll,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,buffer,bufferMemory);

    for(size_t i = 0; i < Count_tex; i++)
    {
        stbi_uc* pixels = aPixels[i];

        void* data;
        vkMapMemory(g_device,bufferMemory,imageSize * i,imageSize,0,&data);
            memcpy(data,aPixels[i],static_cast<size_t>(imageSize));
        vkUnmapMemory(g_device,bufferMemory);
        stbi_image_free(pixels);
    }

    //3. Create image
    vulkan::resource::image::CreateImage(width,height,depth,numArray,
    mipMapCount,VK_IMAGE_TYPE_2D,true,numSamples,format,VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
    VK_IMAGE_USAGE_SAMPLED_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    image,imageMemory);

    //4. Transition image layout and copy staging buffer to it
    VkCommandBuffer cmdBuffer = vulkan::console::command::BeginSingleTimeCommands();
    {
        vulkan::memory::transitionImageLayout(cmdBuffer,image,
        VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        0,1,0,numArray);
        {
            vulkan::resource::image::copyBufferToImage(cmdBuffer,buffer,image,
            static_cast<uint32_t>(width),static_cast<uint32_t>(height),
            static_cast<uint32_t>(depth),numArray);
        }
        vulkan::memory::transitionImageLayout(cmdBuffer,image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        0,1,0,numArray);

        vulkan::resource::image::GenerateMipmaps(cmdBuffer,image,format,
        static_cast<int32_t>(width),static_cast<int32_t>(height),
        mipMapCount,numArray,autoMipmap);
    }

    vulkan::console::command::EndSingleTimeCommands(cmdBuffer);
    return true;
}






