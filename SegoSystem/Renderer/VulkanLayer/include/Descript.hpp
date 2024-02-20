#include "../../Vulkan/include/Vk_Descriptor.h"

/*
DescriptorManager Setps BindLayout -> Set -> BindBufferData -> UpdateSet
*/
namespace Sego::Vulkan
{
   class DescriptorManager
   {
    public:
        DescriptorManager();
        ~DescriptorManager();
        DescriptorManager& operator=(const DescriptorManager&) = delete;
        DescriptorManager(const DescriptorManager&) = delete;
        DescriptorManager& operator=(DescriptorManager&&) = delete;

        void BindLayout(vulkan::resource::descriptor::LayoutType type);
       
        void Set(uint32_t desCount);
        void Set(uint32_t desCount,uint32_t maxSets);

        void BindBufferData(VkDescriptorImageInfo& imageInfos,VkDescriptorBufferInfo& bufferinfo
        ,vulkan::resource::descriptor::DescriptorType types,uint32_t setIndex);

        void UpdateSet();

        
        //Input
        VkDescriptorSetLayout& GetDescriptorSetLayouts() { return m_DescriptorSetLayouts; }
        std::vector<VkDescriptorSet>& GetDescriptorSets() { return m_DescriptorSets; }

    private:
        //Base
        std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayoutBindings;
        std::vector<VkDescriptorPoolSize> m_DescriptorPoolSizes;
        std::vector<VkWriteDescriptorSet> m_DescriptorWrites;
        //Descriptor
        VkDescriptorPool m_DescriptorPools;
        VkDescriptorSetLayout m_DescriptorSetLayouts;
        std::vector<VkDescriptorSet> m_DescriptorSets;
        int bindingindex = 0;
   };

}