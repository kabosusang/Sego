#include "pch.h"

#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Allocate.h"
#include "VK_Global_Data.h"
//TextureImage
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>


//

/*************Texture************************
 * CreateTexture
 * CreateTextureView
 * SGvk_Device_Create_TextureSampler
*/
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Vk_Create_Resource.h"

void SG_CRes::CreateTexture(
Texture2D* texture2d,
std::string Image_path,
VkDevice& device,
VkPhysicalDevice& physicalDevice,
VkCommandPool& commandPool,
VkQueue& graQue
)
{
int texChannels;

stbi_uc* pixels = stbi_load(Image_path.c_str(),
&texture2d->wdith_,&texture2d->height_,&texChannels,
STBI_rgb_alpha);

VkDeviceSize imageSize = texture2d->wdith_ * texture2d->height_ * 4;
    if (!pixels) {
        SG_CORE_ERROR("failed to load texture image!");
    }
        //根据颜色通道数，判断颜色格式。
        switch (texChannels) {
            case 1:
            {
                texture2d->texture_format_ = texformat::TX_ALPHA;
                break;
            }
            case 3:
            {
                texture2d->texture_format_ = texformat::TX_RGB;
                break;
            }
            case 4:
            {
                texture2d->texture_format_ = texformat::TX_RGBA;
                break;
            }
        }

VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;
SG_Allocate::SGvk_Device_Create_Buffer(imageSize, 
VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
stagingBuffer, 
stagingBufferMemory,device,physicalDevice);

void* data;
vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
memcpy(data, pixels, static_cast<size_t>(imageSize));
vkUnmapMemory(device, stagingBufferMemory);

stbi_image_free(pixels);
//设置纹理数据

//设置mip层数
texture2d->mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texture2d->wdith_,texture2d->height_)))) + 1;

SG_Allocate::SGvk_Device_Create_Image(physicalDevice,device,texture2d->wdith_,
texture2d->height_,texture2d->mipLevels,
VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, 
VK_IMAGE_USAGE_TRANSFER_SRC_BIT |VK_IMAGE_USAGE_TRANSFER_DST_BIT| VK_IMAGE_USAGE_SAMPLED_BIT, 
VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
texture2d->textureImage, texture2d->textureImageMemory);

//opyBufferToImage
SG_Allocate::SGvk_Device_Create_TransitionImageLayout(graQue,device,commandPool,
texture2d->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, 
VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,texture2d->mipLevels);

SG_Allocate::copyBufferToImage(graQue,device,commandPool,stagingBuffer, 
texture2d->textureImage, static_cast<uint32_t>(texture2d->wdith_), 
static_cast<uint32_t>(texture2d->height_));

/*
SG_Allocate::SGvk_Device_Create_TransitionImageLayout(graQue,device,commandPool,
Te_it->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,Te_it->mipLevels);
 */

vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

//Mipmap
GenerateMipmaps(device,commandPool,graQue,texture2d->textureImage, 
VK_FORMAT_R8G8B8A8_SRGB,physicalDevice
,static_cast<uint32_t>(texture2d->wdith_)
,static_cast<uint32_t>(texture2d->height_),texture2d->mipLevels);

}



void SG_CRes::GenerateMipmaps(VkDevice& device,VkCommandPool& commandPool,VkQueue& graQue,
    VkImage image,VkFormat imageFormat,VkPhysicalDevice& physicalDevice,
     int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
{
    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    SG_CORE_ERROR("texture image format does not support linear blitting!");
}


    VkCommandBuffer commandBuffer = SG_COMD::beginSingleTimeCommands(device,commandPool);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    //loop record VkCmdBlitImage
    for (uint32_t i = 1; i < mipLevels; i++) {
    barrier.subresourceRange.baseMipLevel = i - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    VkImageBlit blit{};
    blit.srcOffsets[0] = { 0, 0, 0 };
    blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.srcSubresource.mipLevel = i - 1;
    blit.srcSubresource.baseArrayLayer = 0;
    blit.srcSubresource.layerCount = 1;
    blit.dstOffsets[0] = { 0, 0, 0 };
    blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.dstSubresource.mipLevel = i;
    blit.dstSubresource.baseArrayLayer = 0;
    blit.dstSubresource.layerCount = 1;

    vkCmdBlitImage(commandBuffer,
    image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1, &blit,
    VK_FILTER_LINEAR);

    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;


    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);


    SG_COMD::endSingleTimeCommands(graQue,device,commandBuffer,commandPool);
}

void SG_CRes::CreateTextureView(VkDevice& device,VkImage& textureImage,
VkImageView& textureImageView,uint32_t mipLevels)
{
textureImageView = SGvk_Device_Create_ImageView_AttachFuc(device,textureImage, 
VK_FORMAT_R8G8B8A8_SRGB,VK_IMAGE_ASPECT_COLOR_BIT,mipLevels);
}

VkImageView SG_CRes::SGvk_Device_Create_ImageView_AttachFuc(VkDevice& device,VkImage& image, VkFormat format,
VkImageAspectFlags aspectFlags,uint32_t mipLevels)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture image view!");
    }

    return imageView;
}

void SG_CRes::SGvk_Device_Create_TextureSampler(VkDevice& device,
VkPhysicalDevice& physicalDevice,VkSampler& textureSampler,uint32_t mipLevels)
{
VkSamplerCreateInfo samplerInfo{};
samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
samplerInfo.magFilter = VK_FILTER_LINEAR;
samplerInfo.minFilter = VK_FILTER_LINEAR;

samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

VkPhysicalDeviceProperties properties{};
vkGetPhysicalDeviceProperties(physicalDevice, &properties);

//下面两个是否应用各向异性过滤 值越低性能越好
samplerInfo.anisotropyEnable = VK_TRUE;
samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;//设备支持最高值
//该字段指定在采样超出范围时返回的颜色 具有钳位到边框寻址模式的图像。可以返回黑色， 浮动或整数格式的白色或透明。不能指定 任意颜色
samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
//该字段指定所需的坐标系 用于寻址图像中的纹素
samplerInfo.unnormalizedCoordinates = VK_FALSE;

samplerInfo.compareEnable = VK_FALSE;
samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
samplerInfo.mipLodBias = 0.0f;
samplerInfo.minLod = 0.0f;
samplerInfo.maxLod = static_cast<float>(mipLevels);

 if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
       SG_CORE_ERROR("failed to create texture sampler!");
    }
}


//MeshVertex
void SG_CRes::SGvk_Device_Create_VertexBuffer(std::vector<MeshVertex> &vertices, VkBuffer &vertexBuffer, VkDeviceMemory &vertexBufferMemory, VkDevice &device, VkPhysicalDevice &physicalDevice, VkCommandPool &cmdPool, VkQueue &endque)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    //临时缓冲区
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
    stagingBuffer, stagingBufferMemory,device,physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory,device,physicalDevice);

    SG_Allocate::copyBuffer(stagingBuffer, vertexBuffer, bufferSize,device,cmdPool,endque);
    //清理
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

}

void SG_CRes::SGvk_Device_Create_IndexBuffer(std::vector<uint32_t> &indices32, VkBuffer &indexBuffer, VkDeviceMemory &indexBufferMemory, VkDevice &device, VkPhysicalDevice &physicaldevice, VkCommandPool &cmdPool, VkQueue &endque)
{
VkDeviceSize bufferSize = sizeof(indices32[0]) * indices32.size();

VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;
SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory,device,physicaldevice);

void* data;
vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
memcpy(data, indices32.data(), (size_t) bufferSize);
vkUnmapMemory(device, stagingBufferMemory);

SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory,device,physicaldevice);

SG_Allocate::copyBuffer(stagingBuffer, indexBuffer, bufferSize,device,cmdPool,endque);


vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

}

///     Uniform Data
///
//
//
//



void SG_CRes::SGvk_Device_Create_DescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout)
{
//obj-uniform
VkDescriptorSetLayoutBinding uboLayoutBinding{};
uboLayoutBinding.binding = 0;
uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
uboLayoutBinding.descriptorCount = 1;
uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;//指定在VERTEX阶段使用
uboLayoutBinding.pImmutableSamplers = nullptr;//跟预采样有关

//Texture2D
VkDescriptorSetLayoutBinding samplerLayoutBinding{};
samplerLayoutBinding.binding = 1;
samplerLayoutBinding.descriptorCount = 1;
samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
samplerLayoutBinding.pImmutableSamplers = nullptr;
samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;//指定在Fragment阶段使用
//绑定描述

std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
VkDescriptorSetLayoutCreateInfo layoutInfo{};
layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
layoutInfo.pBindings = bindings.data();


if (vkCreateDescriptorSetLayout(g_device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create descriptor set layout!");
}

}

void SG_CRes::SGvk_CreateUniformBuffers(std::vector<VkBuffer>& Obj_uniformBuffers,
std::vector<VkDeviceMemory>& Obj_uniformBuffersMemory,
std::vector<void*>& Obj_uniformBuffersMapped)
{

VkDeviceSize bufferSize = sizeof(UniformBufferObject);

Obj_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
Obj_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
Obj_uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
{
    SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, 
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

    , Obj_uniformBuffers[i],Obj_uniformBuffersMemory[i],g_device,g_physicalDevice);

    vkMapMemory(g_device, Obj_uniformBuffersMemory[i], 0, bufferSize, 0, &Obj_uniformBuffersMapped[i]);
}
}



//DescriptorSets
void SG_CRes::SGvk_CreateDescriptorSets(
VkDescriptorSetLayout& descriptorSetLayout,
VkDescriptorPool& descriptorPool,
std::vector<VkDescriptorSet>& Obj_DescriptorSets,
std::vector<Texture2D*>& texs,
std::vector<VkBuffer>& Obj_uniformBuffers,
std::vector<VkDeviceMemory>& Obj_uniformBuffersMemory,
std::vector<void*>& Obj_uniformBuffersMapped)
{
std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
VkDescriptorSetAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
allocInfo.descriptorPool = descriptorPool;
allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
allocInfo.pSetLayouts = layouts.data();
Obj_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
VkResult err;
err = vkAllocateDescriptorSets(g_device, &allocInfo, Obj_DescriptorSets.data());
if (err != VK_SUCCESS) {
    SG_CORE_ERROR("failed to allocate descriptor sets!");
    SG_CORE_ERROR("Error CODE : {0}",err);
}

std::vector<VkDescriptorImageInfo> imageInfos;

for (auto tex : texs)
{
    imageInfos.push_back(tex->descriptor);
}
for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
VkDescriptorBufferInfo bufferInfo{};
bufferInfo.buffer =  Obj_uniformBuffers[i];
bufferInfo.offset = 0;
bufferInfo.range = sizeof(UniformBufferObject);

std::array<VkWriteDescriptorSet,2> descriptorWrites{};
descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
descriptorWrites[0].dstSet = Obj_DescriptorSets[i];
descriptorWrites[0].dstBinding = 0;
descriptorWrites[0].dstArrayElement = 0;
descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
descriptorWrites[0].descriptorCount = 1;
descriptorWrites[0].pBufferInfo = &bufferInfo;
descriptorWrites[0].pImageInfo = nullptr; // Optional
descriptorWrites[0].pTexelBufferView = nullptr; // Optional

descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
descriptorWrites[1].dstSet = Obj_DescriptorSets[i];
descriptorWrites[1].dstBinding = 1;
descriptorWrites[1].dstArrayElement = 0;
descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
descriptorWrites[1].descriptorCount = static_cast<uint32_t>(imageInfos.size());
descriptorWrites[1].pImageInfo = imageInfos.data();

vkUpdateDescriptorSets(g_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

}
}


