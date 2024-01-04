#pragma once

#include <vector>
#include <optional>
#include <string>

#include "Vertex/Vertex.h"
#include "Model/Model.h"

#define MAX_FRAMES_IN_FLIGHT 2

namespace SG_CRes
{   
    void CreateTexture(std::vector<SG_Texture>::iterator it,
    VkDevice& device,VkPhysicalDevice& physicalDevice,VkCommandPool& commandPool,
    VkQueue& graQue);

    
    void GenerateMipmaps(VkDevice& device,VkCommandPool& commandPool,VkQueue& graQue,
    VkImage image,VkFormat imageFormat,VkPhysicalDevice& physicalDevice,
     int32_t texWidth, int32_t texHeight, uint32_t mipLevels);


    void CreateTextureView(VkDevice& device,VkImage& textureImage,VkImageView& textureImageView,uint32_t mipLevels);
    VkImageView SGvk_Device_Create_ImageView_AttachFuc(VkDevice& device,
    VkImage& image, VkFormat format,VkImageAspectFlags aspectFlags,uint32_t mipLevels);

    void SGvk_Device_Create_TextureSampler(VkDevice& device,
    VkPhysicalDevice& physicalDevice,VkSampler& textureSampler,uint32_t mipLevels);

    //LoadModel
    void loadModel_tiny_obj(std::vector<SG_Model>::iterator m_it);
    void loadModel_mesh(std::vector<SG_Model>::iterator m_it);

    void SGvk_Device_Create_VertexBuffer(std::vector<SG_Model>::iterator,VkDevice& device,
    VkPhysicalDevice& physicalDevice,VkCommandPool& cmdPool, VkQueue &endque);

    void SGvk_Device_Create_IndexBuffer(std::vector<SG_Model>::iterator,VkDevice& device,
    VkPhysicalDevice& physicaldevice,VkCommandPool &cmdPool, VkQueue &endque);




//Uniform

    void SGvk_CreateUniformBuffers(std::vector<SG_Model>& models);
    void SGvk_CreateDescriptorSets(VkDescriptorSetLayout& descriptorSetLayout,
    VkDescriptorPool& descriptorPool,std::vector<SG_Model>& models);












};









