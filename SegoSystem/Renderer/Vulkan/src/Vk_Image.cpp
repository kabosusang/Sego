#include "pch.h"

#include "VK_Global_Data.h"
#include "../include/Vk_Image.h"
#include "../include/Vk_Console.h"
#include "../include/Vk_Memory.h"

void vulkan::resource::image::CreateImage(uint32_t width, uint32_t height, 
uint32_t depth, uint32_t numArray, uint32_t mipMapCount, 
VkImageType type, bool isCubeMap, VkSampleCountFlagBits numSamples, 
VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    if (isCubeMap)
        imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    imageInfo.imageType = type;
    imageInfo.format = format;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = depth;
    imageInfo.mipLevels = mipMapCount <= 0 ? 1 : mipMapCount;
    imageInfo.arrayLayers = numArray;
    imageInfo.samples = numSamples;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if(vkCreateImage(g_device ,&imageInfo,nullptr,&image) != VK_SUCCESS)
    {
        SG_CORE_ERROR("TextureCommon::CreateImage Error:Failed to create image !");
    }
    //缓冲区分配
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(g_device, image, &memRequirements);//获取到图像对象image所需的内存类型索引和内存大小等信息

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vulkan::memory::findMemoryType(memRequirements.memoryTypeBits, 
    properties);

    if (vkAllocateMemory(g_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to allocate image memory!");
    }
    vkBindImageMemory(g_device, image, imageMemory, 0);
}

void vulkan::resource::image::CreateImageView(VkImage image, 
VkImageViewType type, VkFormat format, VkImageAspectFlags 
aspectFlags, uint32_t mipMapCount, uint32_t numArray, VkImageView &imageview)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = type;
    viewInfo.format = format;
    viewInfo.components = { VK_COMPONENT_SWIZZLE_R ,VK_COMPONENT_SWIZZLE_G,VK_COMPONENT_SWIZZLE_B,VK_COMPONENT_SWIZZLE_A};

    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipMapCount;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = numArray;

    if (vkCreateImageView(g_device, &viewInfo, nullptr, &imageview) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture image view!");
    }

}

void vulkan::resource::image::CreateSampler(VulkanTextureFilterType texFilter, 
VulkanTextureAddressingType texAddressing, 
VulkanTextureBorderColorType texBorderColor,
bool enableAnisotropy, float minLod, float maxLod, float mipLodBias, 
VkSampler &sampler)
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.minFilter = TransformVkFilter(texFilter,VulkanTextureFilterSize_Min);
    samplerInfo.magFilter = TransformVkFilter(texFilter,VulkanTextureFilterSize_Mag);

    samplerInfo.addressModeU = TransformVkSamplerAddressMode(texAddressing);
    samplerInfo.addressModeV = TransformVkSamplerAddressMode(texAddressing);
    samplerInfo.addressModeW = TransformVkSamplerAddressMode(texAddressing);
    //下面两个是否应用各向异性过滤 值越低性能越好
    samplerInfo.anisotropyEnable = enableAnisotropy ? VK_TRUE : VK_FALSE;
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(g_physicalDevice, &properties);
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;//设备支持最高值
    samplerInfo.borderColor = Util_Transform2VkBorderColor(texBorderColor);

    //该字段指定在采样超出范围时返回的颜色 具有钳位到边框寻址模式的图像。可以返回黑色， 浮动或整数格式的白色或透明。不能指定 任意颜色
    //该字段指定所需的坐标系 用于寻址图像中的纹素
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    //Mipmap
    samplerInfo.mipmapMode = TransformVkSamplerMipmapMpde(texFilter);
    samplerInfo.minLod = minLod;
    samplerInfo.maxLod = maxLod;
    samplerInfo.mipLodBias = mipLodBias;

    if (vkCreateSampler(g_device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture sampler!");
    }
}
#include "../include/Vk_Memory.h"
void vulkan::resource::image::GenerateMipmaps(VkCommandBuffer cmdBuffer, VkImage image, VkFormat VkImageFormatListCreateInfo, int32_t width, int32_t height, uint32_t mipMapCount, uint32_t numArray, bool autoMipMap)
{
    bool isCreate = false;
    if(cmdBuffer == VK_NULL_HANDLE)
    {
        isCreate = true;
        cmdBuffer = vulkan::console::command::BeginSingleTimeCommands();
    }

    {

        if(autoMipMap)
        {
            auto mipWidth = width;
            auto mipHeight = height;
            for (uint32_t i = 1; i < mipMapCount; i++)
            {
                vulkan::memory::transitionImageLayout(cmdBuffer,image,
                VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                i,
                1,
                0,
                numArray);

                {
                    VkImageBlit blit{};
                    blit.srcOffsets[0] = {0,0,0};
                    blit.srcOffsets[1] = {mipWidth,mipHeight,1};
                    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    blit.srcSubresource.mipLevel = i - 1;
                    blit.srcSubresource.baseArrayLayer = 0;
                    blit.srcSubresource.layerCount = numArray;

                    blit.dstOffsets[0] = {0,0,0};
                    blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1,mipHeight > 1 ? mipHeight / 2 :1,1};
                    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    blit.dstSubresource.mipLevel = i;
                    blit.dstSubresource.baseArrayLayer = 0;
                    blit.dstSubresource.layerCount = numArray;

                    vkCmdBlitImage(cmdBuffer,image,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    image,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,&blit,VK_FILTER_LINEAR);
                    if(mipWidth > 1)
                        mipWidth /= 2;
                    if(mipHeight >1)
                        mipHeight /=2;
                }

                vulkan::memory::transitionImageLayout(cmdBuffer,image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                i,
                1,
                0,
                numArray);

            }
        }

        vulkan::memory::transitionImageLayout(cmdBuffer,image,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                0,
                mipMapCount,
                0,
                numArray);
    }

    if(isCreate)
    {
        vulkan::console::command::EndSingleTimeCommands(cmdBuffer);
    }

}


//transform
VkFilter vulkan::resource::image::TransformVkFilter(VulkanTextureFilterType typeFilter,VulkanTextureFilterSizeType typeFilterSize)
{
    switch((int)typeFilter)
    {
    case Vulkan_TextureFilter_None:
        {
            switch ((int)typeFilterSize)
            {
            case VulkanTextureFilterSize_Min:   return VK_FILTER_NEAREST;
            case VulkanTextureFilterSize_Mag:   return VK_FILTER_NEAREST;
            }
        }
        break;

    case Vulkan_TextureFilter_Bilinear:
        {
            switch ((int)typeFilterSize)
            {
            case VulkanTextureFilterSize_Min:   return VK_FILTER_LINEAR;
            case VulkanTextureFilterSize_Mag:   return VK_FILTER_LINEAR;
            }
        }
        break;
    case Vulkan_TextureFilter_Trilinear:
    {
        switch ((int)typeFilterSize)
        {
        case VulkanTextureFilterSize_Min:   return VK_FILTER_LINEAR;
        case VulkanTextureFilterSize_Mag:   return VK_FILTER_LINEAR;
        }
    }
    break;

    case Vulkan_TextureFilter_Anisotropic:
    {
        switch ((int)typeFilterSize)
        {
        case VulkanTextureFilterSize_Min:   return VK_FILTER_LINEAR;
        case VulkanTextureFilterSize_Mag:   return VK_FILTER_LINEAR;
        }
    }
    break;
    }
    return VK_FILTER_NEAREST;
}

VkSamplerMipmapMode vulkan::resource::image::TransformVkSamplerMipmapMpde(VulkanTextureFilterType typeFilter)
{
    switch((int)typeFilter)
    {
    case Vulkan_TextureFilter_None:         return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    case Vulkan_TextureFilter_Bilinear:     return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    case Vulkan_TextureFilter_Trilinear:         return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    case Vulkan_TextureFilter_Anisotropic:         return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    }

    return VK_SAMPLER_MIPMAP_MODE_NEAREST;
}

VkSamplerAddressMode vulkan::resource::image::TransformVkSamplerAddressMode(VulkanTextureAddressingType addtype)
{
    
    switch((int)addtype)
    {
    case Vulkan_TextureAddressing_Wrap:             return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case Vulkan_TextureAddressing_Mirror:           return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case Vulkan_TextureAddressing_Clamp:            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case Vulkan_TextureAddressing_Border:           return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    }
    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
}

VkBorderColor vulkan::resource::image::Util_Transform2VkBorderColor(VulkanTextureBorderColorType type)
 {
    switch((int)type)
    {
        case Vulkan_TextureBorderColor_OpaqueBlack:         return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        case Vulkan_TextureBorderColor_OpaqueWhite:         return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
        case Vulkan_TextureBorderColor_TransparentBlack:    return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    }
    return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
}


//复制缓冲数据到图像
void vulkan::resource::image::copyBufferToImage(VkCommandBuffer cmdBuffer, 
VkBuffer buffer, VkImage image, uint32_t width, 
uint32_t height, uint32_t depth, uint32_t numArray)
{

    bool isCreate = false;
    if(cmdBuffer == VK_NULL_HANDLE)
    {
        isCreate = true;
        cmdBuffer == vulkan::console::command::BeginSingleTimeCommands();
    }
    
    {
        std::vector<VkBufferImageCopy> bufferCopyRegions;
        for (uint32_t i = 0; i < numArray; i++)
        {
            VkBufferImageCopy region = {};
            region.bufferOffset = width * height * 4 * i ;
            region.imageExtent.width = width;
            region.imageExtent.height = height;
            region.imageExtent.depth = depth;
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = i;
            region.imageSubresource.layerCount = 1;
            bufferCopyRegions.push_back(region);
        }
        vkCmdCopyBufferToImage(cmdBuffer,buffer,image,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        ,static_cast<uint32_t>(bufferCopyRegions.size()),bufferCopyRegions.data());
    }
    if(isCreate)
    {
        vulkan::console::command::EndSingleTimeCommands(cmdBuffer);
    }
}








