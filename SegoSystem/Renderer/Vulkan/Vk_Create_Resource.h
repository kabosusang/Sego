#pragma once

#include "pch.h"

#include "Vertex/Vertex.h"
#include "Model/Model.h"
#include "Vertex/MeshVertex.h"

#define MAX_FRAMES_IN_FLIGHT 2

namespace  SG_CRes
{   
//Texture

//texture
void CreateTexture(
Texture2D* texture2d,
std::string Image_path,
VkDevice& device,
VkPhysicalDevice& physicalDevice,
VkCommandPool& commandPool,
VkQueue& graQue
);


void GenerateMipmaps(VkDevice& device,VkCommandPool& commandPool,VkQueue& graQue,
VkImage image,VkFormat imageFormat,VkPhysicalDevice& physicalDevice,
int32_t texWidth, int32_t texHeight, uint32_t mipLevels);


void CreateTextureView(VkDevice& device,VkImage& textureImage,VkImageView& textureImageView,uint32_t mipLevels);
VkImageView SGvk_Device_Create_ImageView_AttachFuc(VkDevice& device,
VkImage& image, VkFormat format,VkImageAspectFlags aspectFlags,uint32_t mipLevels);

void SGvk_Device_Create_TextureSampler(VkDevice& device,
VkPhysicalDevice& physicalDevice,VkSampler& textureSampler,uint32_t mipLevels);

//Vertex
void SGvk_Device_Create_VertexBuffer(std::vector<MeshVertex>& vertices,
    VkBuffer& vertexBuffer,VkDeviceMemory& vertexBufferMemory,
    VkDevice& device,
    VkPhysicalDevice& physicalDevice,
    VkCommandPool& cmdPool, 
    VkQueue &endque);

    void SGvk_Device_Create_IndexBuffer(std::vector<uint32_t>& indices32,
    VkBuffer& indexBuffer,VkDeviceMemory& indexBufferMemory,
    VkDevice& device,
    VkPhysicalDevice& physicaldevice,
    VkCommandPool &cmdPool, 
    VkQueue &endque);



//Uniform
//Layout
void SGvk_Device_Create_DescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout);

void SGvk_CreateUniformBuffers(std::vector<VkBuffer>& Obj_uniformBuffers_,
std::vector<VkDeviceMemory>& Obj_uniformBuffersMemory_,
std::vector<void*>& Obj_uniformBuffersMapped_);

void SGvk_CreateDescriptorSets(
VkDescriptorSetLayout& descriptorSetLayout,
VkDescriptorPool& descriptorPool,
std::vector<VkDescriptorSet>& Obj_DescriptorSets,
std::vector<Texture2D*>& texs,
std::vector<VkBuffer>& Obj_uniformBuffers,
std::vector<VkDeviceMemory>& Obj_uniformBuffersMemory,
std::vector<void*>& Obj_uniformBuffersMapped);





};









