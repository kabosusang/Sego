#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Allocate.h"

//TextureImage

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

#include <Log/Log.h>
SG_Model::SG_Model(std::string path,std::string name)
{
Model_Path = path;
Model_Name = name;

}

/*************Texture************************
 * CreateTexture
 * CreateTextureView
 * SGvk_Device_Create_TextureSampler
*/
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void SG_CRes::CreateTexture(
std::vector<SG_Texture>::iterator Te_it,
VkDevice& device,
VkPhysicalDevice& physicalDevice,
VkCommandPool& commandPool,
VkQueue& graQue
)
{
int texChannels;

stbi_uc* pixels = stbi_load(Te_it->Texture_Path.c_str(),
&Te_it->Texture_Attribute.wdith_,&Te_it->Texture_Attribute.height_,&texChannels,
STBI_rgb_alpha);

VkDeviceSize imageSize = Te_it->Texture_Attribute.wdith_ * Te_it->Texture_Attribute.height_ * 4;
    if (!pixels) {
        SG_CORE_ERROR("failed to load texture image!");
    }
        //根据颜色通道数，判断颜色格式。
        switch (texChannels) {
            case 1:
            {
                Te_it->Texture_Attribute.texture_format_ = texformat::TX_ALPHA;
                break;
            }
            case 3:
            {
                Te_it->Texture_Attribute.texture_format_ = texformat::TX_RGB;
                break;
            }
            case 4:
            {
                Te_it->Texture_Attribute.texture_format_ = texformat::TX_RGBA;
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
Te_it->mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(Te_it->Texture_Attribute.wdith_,Te_it->Texture_Attribute.height_)))) + 1;

SG_Allocate::SGvk_Device_Create_Image(physicalDevice,device,Te_it->Texture_Attribute.wdith_, Te_it->Texture_Attribute.height_,VK_FORMAT_R8G8B8A8_SRGB, 
VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT
 | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
 Te_it->textureImage, Te_it->textureImageMemory);

SG_Allocate::SGvk_Device_Create_TransitionImageLayout(graQue,device,commandPool,Te_it->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
SG_Allocate::copyBufferToImage(graQue,device,commandPool,stagingBuffer, Te_it->textureImage, static_cast<uint32_t>(Te_it->Texture_Attribute.wdith_), static_cast<uint32_t>(Te_it->Texture_Attribute.height_));
SG_Allocate::SGvk_Device_Create_TransitionImageLayout(graQue,device,commandPool,Te_it->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
 
vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

}

void SG_CRes::CreateTextureView(VkDevice& device,VkImage& textureImage,VkImageView& textureImageView)
{
textureImageView = SGvk_Device_Create_ImageView_AttachFuc(device,textureImage, VK_FORMAT_R8G8B8A8_SRGB,VK_IMAGE_ASPECT_COLOR_BIT);
}

VkImageView SG_CRes::SGvk_Device_Create_ImageView_AttachFuc(VkDevice& device,VkImage& image, VkFormat format,VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        SG_CORE_ERROR("failed to create texture image view!");
    }

    return imageView;
}

void SG_CRes::SGvk_Device_Create_TextureSampler(VkDevice& device,VkPhysicalDevice& physicalDevice,VkSampler& textureSampler)
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
samplerInfo.maxLod = 0.0f;

 if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
       SG_CORE_ERROR("failed to create texture sampler!");
    }
}


void SG_CRes::loadModel_tiny_obj(std::vector<SG_Model>::iterator m_it)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, m_it->Model_Path.c_str())) {
        throw std::runtime_error(warn + err);
    }
    std::unordered_map<Vertex,uint32_t> uniqueVertices{};

    //将文件中的所有面合并到一个模型中(遍历所有形状)
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};
            vertex.pos = {
            attrib.vertices[3 * index.vertex_index + 0],
            attrib.vertices[3 * index.vertex_index + 1],
            attrib.vertices[3 * index.vertex_index + 2]
        };

#ifdef GLFW_INCLUDE_VULKAN
   vertex.texCoord = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
        };
#else
vertex.texCoord = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            attrib.texcoords[2 * index.texcoord_index + 1]
        };
#endif

            vertex.color = {0.0f, 1.0f, 1.0f,1.0f};

        if (uniqueVertices.count(vertex) == 0){
            uniqueVertices[vertex] = static_cast<uint32_t>(m_it->vertices.size());
            m_it->vertices.push_back(vertex);
        }
            m_it->indices.push_back(uniqueVertices[vertex]);
        }
    }
}
//mesh

void SG_CRes::loadModel_mesh(std::vector<SG_Model>::iterator m_it)
{
    
    MeshFilter ms;
    ms.LoadMesh(m_it->Model_Path);
    //Load 
    
    m_it->vertices = std::move(ms.mesh()->vertex_data_);
    //m_it->indices = std::move(ms.mesh()->vertex_index_data_);
    m_it->indices.resize(ms.mesh()->vertex_index_num_);
    
   std::transform(ms.mesh()->vertex_index_data_.begin(), ms.mesh()->vertex_index_data_.end(), 
   std::back_inserter(m_it->indices),
    [](unsigned short value) {
        return static_cast<uint32_t>(value);
    });
    


}


void SG_CRes::SGvk_Device_Create_VertexBuffer(std::vector<SG_Model>::iterator m_it,
VkDevice& device,VkPhysicalDevice& physicalDevice,VkCommandPool& cmdPool, VkQueue &endque)
{
VkDeviceSize bufferSize = sizeof(m_it->vertices[0]) * m_it->vertices.size();
//临时缓冲区
VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;
SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
stagingBuffer, stagingBufferMemory,device,physicalDevice);


void* data;
vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
memcpy(data, m_it->vertices.data(), (size_t) bufferSize);
vkUnmapMemory(device, stagingBufferMemory);

SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_it->vertexBuffer, m_it->vertexBufferMemory,device,physicalDevice);


SG_Allocate::copyBuffer(stagingBuffer, m_it->vertexBuffer, bufferSize,device,cmdPool,endque);
//清理
vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

}


void SG_CRes::SGvk_Device_Create_IndexBuffer(std::vector<SG_Model>::iterator m_it,VkDevice& device,
VkPhysicalDevice& physicaldevice,VkCommandPool &cmdPool, VkQueue &endque)
{

VkDeviceSize bufferSize = sizeof(m_it->indices[0]) * m_it->indices.size();

VkBuffer stagingBuffer;
VkDeviceMemory stagingBufferMemory;
SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory,device,physicaldevice);

void* data;
vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
memcpy(data, m_it->indices.data(), (size_t) bufferSize);
vkUnmapMemory(device, stagingBufferMemory);

SG_Allocate::SGvk_Device_Create_Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_it->indexBuffer, m_it->indexBufferMemory,device,physicaldevice);

SG_Allocate::copyBuffer(stagingBuffer, m_it->indexBuffer, bufferSize,device,cmdPool,endque);


vkDestroyBuffer(device, stagingBuffer, nullptr);
vkFreeMemory(device, stagingBufferMemory, nullptr);

}



