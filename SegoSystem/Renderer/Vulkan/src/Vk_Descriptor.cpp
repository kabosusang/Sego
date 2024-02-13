#include "pch.h"
#include "../include/Vk_Descriptor.h"
#include "VK_Global_Data.h"


void vulkan::resource::descriptor::CreateDesLayout(VkDescriptorSetLayout & des,
                    std::vector<VkDescriptorSetLayoutBinding>& bindings)
{
VkDescriptorSetLayoutCreateInfo layoutInfo{};
layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
layoutInfo.pBindings = bindings.data();
if (vkCreateDescriptorSetLayout(g_device, &layoutInfo, nullptr,&des) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create descriptor set layout!");
}
}


void vulkan::resource::descriptor::CreateDescriptorPool(VkDescriptorPool &descriptorPool, 
std::vector<VkDescriptorPoolSize> &poolSizes, uint32_t maxSets)
{
VkDescriptorPoolCreateInfo poolInfo{};
poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
poolInfo.pPoolSizes = poolSizes.data();
poolInfo.maxSets = maxSets;

if (vkCreateDescriptorPool(g_device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create descriptor pool!");
}

}
//CreateVkWriteDescriptorSet
VkWriteDescriptorSet vulkan::resource::descriptor::CreateVkWriteDescriptorSet(
VkDescriptorBufferInfo& bufferinfo,
VkDescriptorImageInfo& imageInfos,
DescriptorType types,
VkDescriptorSet& descriptorSet,int bindingindex)
{   
    if(types == DescriptorType::UNIFORM_BUFFER)
    {
        VkWriteDescriptorSet descriptorWrites{};
        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.dstSet = descriptorSet;
        descriptorWrites.dstBinding = bindingindex;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pBufferInfo = &bufferinfo;
        descriptorWrites.pImageInfo = nullptr; // Optional
        descriptorWrites.pTexelBufferView = nullptr; // Optional
        return descriptorWrites;
    }else if(types == DescriptorType::COMBINED_IMAGE_SAMPLER)
    {
        VkWriteDescriptorSet descriptorWrites{};
        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.dstSet = descriptorSet;
        descriptorWrites.dstBinding = bindingindex;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pImageInfo = &imageInfos;
        return descriptorWrites;
    }
    SG_CORE_ERROR("VkWriteDescriptorSet is not supported");
}

//这里使用了descriptorSets.size() 充当MAX_FRAMES_IN_FLIGHT 默认每一帧渲染每一个模型对应一个set
void vulkan::resource::descriptor::CreateDescriptorSets(VkDescriptorSetLayout &desLayout, 
VkDescriptorPool &descriptorPool, std::vector<VkDescriptorSet> &descriptorSets
)
{
std::vector<VkDescriptorSetLayout> layouts(descriptorSets.size(), desLayout);
VkDescriptorSetAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
allocInfo.descriptorPool = descriptorPool;
allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSets.size());
allocInfo.pSetLayouts = layouts.data();
VkResult err;
err = vkAllocateDescriptorSets(g_device, &allocInfo, descriptorSets.data());
if (err != VK_SUCCESS) {
    SG_CORE_ERROR("failed to allocate descriptor sets!");
    SG_CORE_ERROR("Error CODE : {0}",err);
}

}

