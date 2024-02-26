#include "pch.h"
#include "LightRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <rttr/registration>

#include "game_object.h"
#include "transform.h"
#include "MeshFilter/MeshFilter.h"
#include "Core/Material/Material.h"

#include "Renderer/Vulkan/Vk_Create_Resource.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "VulkanStatus/Pipeline.h"
#include "Lights/LightConstans.h"

using namespace vulkan::resource::descriptor;

std::vector<LightRenderer*> Light_renderer;

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<LightRenderer>("LightRenderer")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void LightRenderer::SetMaterial(Material* material) {
    material_=material;
    SG_CORE_INFO("Texture Name : {0}",material_->textures_.data()->first.c_str());

    std::vector<Texture2D*> texture2d;

    // 遍历源向量并提取 Texture2D* 添加到目标向量中
    for (const auto& pair : material_->textures_) {
        texture2d.push_back(pair.second);
    }
    //创建uniform
    obj_uniform_ = new Uniform;
    light_uniform_ = new Uniform;
    RebuildUniform();
    //Set
    descriptorManager_  = new Sego::Vulkan::DescriptorManager();
    descriptorManager_->BindLayout(LayoutType::VERTEX_BUFFER);
    descriptorManager_->BindLayout(LayoutType::FRAGME_IMAGE);
    descriptorManager_->BindLayout(LayoutType::FRAGME_BUFFER);
    descriptorManager_->Set(MAX_FRAMES_IN_FLIGHT); //设置描述符数量
    
    for(auto i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        //ubo
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer =  obj_uniform_->uniformBuffers_[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        //Phone
        VkDescriptorBufferInfo viewposbufferInfo{};
        viewposbufferInfo.buffer = light_uniform_->uniformBuffers_[i];
        viewposbufferInfo.offset = 0;
        viewposbufferInfo.range = sizeof(PhoneConstans);
       
        descriptorManager_->BindBufferData(texture2d[0]->descriptor, 
        bufferInfo,DescriptorType::UNIFORM_BUFFER,i); //0
        
        descriptorManager_->BindBufferData(texture2d[0]->descriptor, 
        bufferInfo,DescriptorType::COMBINED_IMAGE_SAMPLER,i);//1
        
        descriptorManager_->BindBufferData(texture2d[0]->descriptor, 
        viewposbufferInfo,DescriptorType::UNIFORM_BUFFER,i);//2
      
        descriptorManager_->UpdateSet();
    }
    
    Vk_Pipeline_.resize(1);
    Vk_Pipeline_[0].InputPipeLineStatus(PipelineType::Graphics);
    Vk_Pipeline_[0].CreateGraphicsPipeline_light(material_->vshader_path_[0],
    material_->fshader_path_[0],descriptorManager_->GetDescriptorSetLayouts());
}

LightRenderer::LightRenderer()
{
    
}
#include "VK_Global_Data.h"


LightRenderer::~LightRenderer()
{
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    //Models
    vkDestroyBuffer(g_device,mesh_filter->indexBuffer,nullptr);
    vkFreeMemory(g_device, mesh_filter->indexBufferMemory , nullptr);

    vkDestroyBuffer(g_device,mesh_filter->vertexBuffer, nullptr);//清理缓冲区
    vkFreeMemory(g_device, mesh_filter->vertexBufferMemory, nullptr);

    //Destroy Pipeline
    for (auto& pipe : Vk_Pipeline_)
    {
        pipe.CleanPipeline();
    }

    delete material_;
    delete obj_uniform_;
    delete light_uniform_;
    delete descriptorManager_;


    material_ = nullptr;
    obj_uniform_ = nullptr;
    light_uniform_ = nullptr;
    descriptorManager_ = nullptr;
}


void LightRenderer::Render(VkCommandBuffer& commandBuffer,uint32_t imageIndex)
{
//主动获取Transform组件 ,计算mvp
auto transform = dynamic_cast<Transform*>(game_object()->GetComponent("Transform"));
if(!transform)
{
    SG_CORE_ERROR("/////////////Failed to GetComponent->Transform");
    return;
}

auto rotation=transform->rotation();
glm::mat4 trans = glm::translate(transform->position());
glm::mat4 scale = glm::scale(transform->scale()); //缩放
glm::mat4 eulerAngleYXZ = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
Model_mat4_ = trans * scale * eulerAngleYXZ;

//主动获取 MeshFilter 组件
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    if(!mesh_filter){
        return;
    }

VkDeviceSize offsets[] = {0};
//SwitchOutLine(OutLine);

for(int i = 0; i < Vk_Pipeline_.size(); i++)
{
vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mesh_filter->vertexBuffer ,offsets);
// 绑定顶点缓冲区
 //绑定图形管道
vkCmdBindPipeline(commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,Vk_Pipeline_[i].GetGraphicsPipeline());

// 绑定描述符集
vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
Vk_Pipeline_[i].GetPipelineLayout(), 0, 1, &descriptorManager_->GetDescriptorSets()[app_device->currentFrame] ,0, nullptr);
vkCmdBindIndexBuffer(commandBuffer, mesh_filter->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
// 绑定索引缓冲区并绘制模型
vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh_filter->mesh()->indices32.size()),instanceCount, 0, 0, 0);
}

}

std::vector<VkDeviceMemory> &LightRenderer::GetuniformBufferMemory_obj()
{
    return obj_uniform_->uniformBuffersMemory_;
}

std::vector<void *> &LightRenderer::GetuniformBuffersMapped_obj()
{
    return obj_uniform_->uniformBuffersMapped_;
}

std::vector<VkDeviceMemory> &LightRenderer::GetuniformBufferMemory_light()
{
    return light_uniform_->uniformBuffersMemory_;
}

std::vector<void *> &LightRenderer::GetuniformBuffersMapped_light()
{
    return light_uniform_->uniformBuffersMapped_;
}

void LightRenderer::RecreatePipeline()
{
    CleanPipeLine();
    
    Vk_Pipeline_.resize(1);
    Vk_Pipeline_[0].InputPipeLineStatus(PipelineType::Graphics);
    Vk_Pipeline_[0].CreateGraphicsPipeline_light(material_->vshader_path_[0],
    material_->fshader_path_[0],descriptorManager_->GetDescriptorSetLayouts());
}

void LightRenderer::CleanPipeLine()
{
    for (auto& pipe : Vk_Pipeline_)
    {
        pipe.CleanPipeline();
    }
    Vk_Pipeline_.clear();
}


void LightRenderer::RebuildUniform()
{
VkDeviceSize bufferSize_obj = sizeof(UniformBufferObject);

obj_uniform_->uniformBuffers_.resize(MAX_FRAMES_IN_FLIGHT);
obj_uniform_->uniformBuffersMemory_.resize(MAX_FRAMES_IN_FLIGHT);
obj_uniform_->uniformBuffersMapped_.resize(MAX_FRAMES_IN_FLIGHT);


VkDeviceSize bufferSize_light = sizeof(PhoneConstans);

light_uniform_->uniformBuffers_.resize(MAX_FRAMES_IN_FLIGHT);
light_uniform_->uniformBuffersMemory_.resize(MAX_FRAMES_IN_FLIGHT);
light_uniform_->uniformBuffersMapped_.resize(MAX_FRAMES_IN_FLIGHT);


for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
{
    SG_Allocate::SGvk_Device_Create_Buffer(bufferSize_obj, 
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    , obj_uniform_->uniformBuffers_[i],
    obj_uniform_->uniformBuffersMemory_[i],g_device,g_physicalDevice);

    vkMapMemory(g_device, obj_uniform_->uniformBuffersMemory_[i], 
    0, bufferSize_obj, 0, &obj_uniform_->uniformBuffersMapped_[i]);

    SG_Allocate::SGvk_Device_Create_Buffer(bufferSize_light, 
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    , light_uniform_->uniformBuffers_[i],
    light_uniform_->uniformBuffersMemory_[i],g_device,g_physicalDevice);

    vkMapMemory(g_device, light_uniform_->uniformBuffersMemory_[i], 
    0, bufferSize_light, 0, &light_uniform_->uniformBuffersMapped_[i]);

}

}

VkSampler & LightRenderer::GetSampler()
{
    return material_->textures_[0].second->textureSampler;
}

VkImageView &LightRenderer::GetImageview()
{
    return material_->textures_[0].second->textureImageView; 
}
