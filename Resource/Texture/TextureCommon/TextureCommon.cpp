#include "pch.h"

#include "TextureCommon.h"
#include "VK_Global_Data.h"

TextureCommon::~TextureCommon()
{
vkDestroySampler(g_device,textureSampler,nullptr);
vkDestroyImageView(g_device,textureImageView,nullptr);

vkDestroyImage(g_device,textureImage, nullptr);
vkFreeMemory(g_device, textureImageMemory, nullptr);

}
#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"

void TextureCommon::LoadImageData(std::string_view Image_path,TextureCommon* texture)
{
int texChannels;
stbi_uc* pixels = stbi_load(Image_path.data(),
&texture->width_,&texture->height_,&texChannels,
STBI_rgb_alpha);

VkDeviceSize imageSize = texture->width_ * texture->height_ * 4;
    if (!pixels) {
        SG_CORE_ERROR("failed to load texture image!");
    }
        //根据颜色通道数，判断颜色格式。
        switch (texChannels) {
            case 1:
            {
                texture->texture_format_ = texColorformat::TX_ALPHA;
                break;
            }
            case 3:
            {
                texture->texture_format_ = texColorformat::TX_RGB;
                break;
            }
            case 4:
            {
                texture->texture_format_ = texColorformat::TX_RGBA;
                break;
            }
        }

}
void TextureCommon::CreateImage()
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    if (isCubeMap)
        imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    imageInfo.imageType = type;
    imageInfo.format = format;
    imageInfo.extent.width = static_cast<uint32_t>(width_);
    imageInfo.extent.height = static_cast<uint32_t>(height_);
    imageInfo.extent.depth = depth_;
    imageInfo.mipLevels = mipLevels <= 0 ? 1 : mipLevels;
    imageInfo.arrayLayers = numArray;
    imageInfo.samples = numSamples;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if(vkCreateImage(g_device ,&imageInfo,nullptr,&textureImage) != VK_SUCCESS)
    {
        SG_CORE_ERROR("TextureCommon::CreateImage Error:Failed to create image !");
    }
    //缓冲区分配
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(g_device, textureImage, &memRequirements);//获取到图像对象image所需的内存类型索引和内存大小等信息

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = SG_Allocate::findMemoryType(memRequirements.memoryTypeBits, 
    properties,g_physicalDevice);

    if (vkAllocateMemory(g_device, &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to allocate image memory!");
    }
    vkBindImageMemory(g_device, textureImage, textureImageMemory, 0);
}

void TextureCommon::CreateImageView()
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = textureImage;
    viewInfo.viewType = viewtype;
    viewInfo.format = format;
    viewInfo.components = { VK_COMPONENT_SWIZZLE_R ,VK_COMPONENT_SWIZZLE_G,VK_COMPONENT_SWIZZLE_B,VK_COMPONENT_SWIZZLE_A};

    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = numArray;

    if (vkCreateImageView(g_device, &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture image view!");
    }
}

VkFilter TextureCommon::TransformVkFilter(VulkanTextureFilterType typeFilter,VulkanTextureFilterSizeType typeFilterSize)
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

VkSamplerMipmapMode TextureCommon::TransformVkSamplerMipmapMpde(VulkanTextureFilterType typeFilter)
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

VkSamplerAddressMode TextureCommon::TransformVkSamplerAddressMode(VulkanTextureAddressingType addtype)
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

VkBorderColor TextureCommon::Util_Transform2VkBorderColor(VulkanTextureBorderColorType type)
 {
    switch((int)type)
    {
        case Vulkan_TextureBorderColor_OpaqueBlack:         return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        case Vulkan_TextureBorderColor_OpaqueWhite:         return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
        case Vulkan_TextureBorderColor_TransparentBlack:    return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    }
    return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
}


void TextureCommon::CreateImageSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.minFilter = TransformVkFilter(texfilter,VulkanTextureFilterSize_Min);
    samplerInfo.magFilter = TransformVkFilter(texfilter,VulkanTextureFilterSize_Mag);

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
    samplerInfo.mipmapMode = TransformVkSamplerMipmapMpde(texfilter);
    samplerInfo.minLod = minLod;
    samplerInfo.maxLod = maxLod;
    samplerInfo.mipLodBias = mipLodBias;

    if (vkCreateSampler(g_device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture sampler!");
    }

}





TextureCommon *TextureCommon::LoadFromFile(std::string &image_file_path)
{
    TextureCommon* texturecommon = new TextureCommon;
   
    
    SG_CRes::CreateTextureView(g_device,texturecommon->textureImage,
    texturecommon->textureImageView,texturecommon->mipLevels);
    SG_CRes::SGvk_Device_Create_TextureSampler(g_device,g_physicalDevice,
    texturecommon->textureSampler,texturecommon->mipLevels);
    
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texturecommon->textureImageView;
    imageInfo.sampler = texturecommon->textureSampler;
    texturecommon->descriptor = imageInfo;

    return texturecommon;

}


