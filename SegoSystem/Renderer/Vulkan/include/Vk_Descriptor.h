#pragma once
#include <vector>
#include <vulkan/vulkan.h>

namespace vulkan::resource::descriptor
{
    enum class DescriptorType
    {
        UNIFORM_BUFFER = 0,
        DYNAMIC_UNIFORM_BUFFER,
        STORAGE_BUFFER,
        STORAGE_IMAGE,
        COMBINED_IMAGE_SAMPLER,
        INPUT_ATTACHMENT
    };
    enum class LayoutType
    {
        VERTEX_BUFFER = 0,
        FRAGME_IMAGE,
        FRAGME_BUFFER,
        VERTEX_IMAGE
    };


    void CreateDesLayout(VkDescriptorSetLayout& des ,
                         std::vector<VkDescriptorSetLayoutBinding>& bindings);  

    void CreateDescriptorPool(VkDescriptorPool& descriptorPool,
                              std::vector<VkDescriptorPoolSize>& poolSizes,
                              uint32_t maxSets);


    VkWriteDescriptorSet CreateVkWriteDescriptorSet(VkDescriptorBufferInfo& bufferinfo,
                            VkDescriptorImageInfo& imageInfos,
                            DescriptorType types,VkDescriptorSet& descriptorSet,int bindingindex);


    void CreateDescriptorSets(VkDescriptorSetLayout& desLayout,
                              VkDescriptorPool& descriptorPool,
                              std::vector<VkDescriptorSet>& descriptorSets //input des is Null
                                );

}


