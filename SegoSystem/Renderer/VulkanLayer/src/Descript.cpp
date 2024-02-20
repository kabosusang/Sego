#include "pch.h"

#include "../include/Descript.hpp"
#include "VK_Global_Data.h"

using namespace vulkan::resource::descriptor;

Sego::Vulkan::DescriptorManager::DescriptorManager()
{
    m_DescriptorSetLayouts = VK_NULL_HANDLE;
    m_DescriptorPools = VK_NULL_HANDLE;
    m_DescriptorSets.resize(0);
}

Sego::Vulkan::DescriptorManager::~DescriptorManager()
{
    for (auto& des : m_DescriptorSets)
    {
    vkFreeDescriptorSets(g_device, m_DescriptorPools, 1, &des);
    }
    m_DescriptorSets.clear();

    vkDestroyDescriptorSetLayout(g_device,m_DescriptorSetLayouts,nullptr);
    vkDestroyDescriptorPool(g_device,m_DescriptorPools,nullptr);
    m_DescriptorPools = VK_NULL_HANDLE;
    m_DescriptorSetLayouts = VK_NULL_HANDLE;
}

void Sego::Vulkan::DescriptorManager::BindLayout(LayoutType type)
{
    if(type == LayoutType::VERTEX_BUFFER)
    {
        VkDescriptorSetLayoutBinding LayoutBinding{};
        LayoutBinding.binding = m_DescriptorSetLayoutBindings.size();
        LayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        LayoutBinding.descriptorCount = 1;
        LayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;//指定在VERTEX阶段使用
        LayoutBinding.pImmutableSamplers = nullptr;//跟预采样有关

        m_DescriptorSetLayoutBindings.emplace_back(LayoutBinding);

        VkDescriptorPoolSize poolSizes{};
        poolSizes.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes.descriptorCount = static_cast<uint32_t>(2);

        m_DescriptorPoolSizes.push_back(poolSizes);
    }
    else if(type == LayoutType::FRAGME_IMAGE)
    {
        VkDescriptorSetLayoutBinding LayoutBinding{};
        LayoutBinding.binding = m_DescriptorSetLayoutBindings.size();
        LayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        LayoutBinding.descriptorCount = 1;
        LayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;//指定在FRAGMENT阶段使用
        LayoutBinding.pImmutableSamplers = nullptr;//跟预采样有关

        m_DescriptorSetLayoutBindings.emplace_back(LayoutBinding);

        VkDescriptorPoolSize poolSizes{};
        poolSizes.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes.descriptorCount = static_cast<uint32_t>(2);

        m_DescriptorPoolSizes.push_back(poolSizes);
    }else if(type == LayoutType::FRAGME_BUFFER)
    {
        VkDescriptorSetLayoutBinding LayoutBinding{};
        LayoutBinding.binding = m_DescriptorSetLayoutBindings.size();
        LayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        LayoutBinding.descriptorCount = 1;
        LayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;//指定在FRAGMENT阶段使用
        LayoutBinding.pImmutableSamplers = nullptr;//跟预采样有关

        m_DescriptorSetLayoutBindings.emplace_back(LayoutBinding);
        m_DescriptorPoolSizes.push_back({VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,2});
    }else
    {
        SG_CORE_ERROR("DescriptorType is not supported!");
        return;
    }
}

void Sego::Vulkan::DescriptorManager::Set(uint32_t desCount)
{
    CreateDesLayout(m_DescriptorSetLayouts,m_DescriptorSetLayoutBindings);

    for (auto& pool : m_DescriptorPoolSizes)
    {
        pool.descriptorCount = desCount;
    }
    m_DescriptorSets.resize(desCount);
    CreateDescriptorPool(m_DescriptorPools, m_DescriptorPoolSizes, desCount);
    CreateDescriptorSets(m_DescriptorSetLayouts,m_DescriptorPools,m_DescriptorSets);
}

void Sego::Vulkan::DescriptorManager::Set(uint32_t desCount,uint32_t maxSets)
{
    m_DescriptorSets.resize(desCount);

    for (auto& pool : m_DescriptorPoolSizes)
    {
        pool.descriptorCount = desCount;
    }

    CreateDescriptorPool(m_DescriptorPools, m_DescriptorPoolSizes, maxSets);
    CreateDescriptorSets(m_DescriptorSetLayouts,m_DescriptorPools,m_DescriptorSets);
}

void Sego::Vulkan::DescriptorManager::BindBufferData(VkDescriptorImageInfo &imageInfos, 
VkDescriptorBufferInfo &bufferinfo, DescriptorType types ,uint32_t setIndex)
{
    VkWriteDescriptorSet descriptorWrites = CreateVkWriteDescriptorSet(bufferinfo,imageInfos,types,m_DescriptorSets[setIndex],bindingindex);
    m_DescriptorWrites.emplace_back(descriptorWrites);
    bindingindex++;
}


void Sego::Vulkan::DescriptorManager::UpdateSet()
{
    SG_CORE_INFO("{0}",m_DescriptorWrites.size());

    vkUpdateDescriptorSets(g_device, static_cast<uint32_t>(m_DescriptorWrites.size()), m_DescriptorWrites.data(), 0, nullptr);
    m_DescriptorWrites.clear();

    bindingindex = 0;
}




