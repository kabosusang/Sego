#include "pch.h"
#include "SGComponent/LightRenderer/LightRenderer.h"
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
    SG_CORE_INFO("Runing00");

    //创建uniform
    obj_uniform_ = new Uniform;
    light_uniform_ = new Uniform;
    RebuildUniform();

    //重新创建
    ReBuildDesLayout();
    RebuildDescriptPool();
    RebuildDescriptorSets(texture2d);

    Vk_Pipeline_.resize(1);
    Vk_Pipeline_[0].InputPipeLineStatus(PipelineType::Graphics);
    Vk_Pipeline_[0].CreateGraphicsPipeline_light(material_->vshader_path_[0],
    material_->fshader_path_[0],descriptorSetLayout_);

}

LightRenderer::LightRenderer()
{

}
#include "VK_Global_Data.h"
#include "LightRenderer.h"

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

    vkDestroyDescriptorSetLayout(g_device,descriptorSetLayout_,nullptr);
    vkDestroyDescriptorPool(g_device,descriptorPool_,nullptr);


    delete material_;
    delete obj_uniform_;
    delete light_uniform_;
    
    material_ = nullptr;
    obj_uniform_ = nullptr;
    light_uniform_ = nullptr;
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
Vk_Pipeline_[i].GetPipelineLayout(), 0, 1, &descriptorSets_[app_device->currentFrame] ,0, nullptr);
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
    material_->fshader_path_[0],descriptorSetLayout_);
}

void LightRenderer::CleanPipeLine()
{
    for (auto& pipe : Vk_Pipeline_)
    {
        pipe.CleanPipeline();
    }
    Vk_Pipeline_.clear();
}


//RebuildDescript


void LightRenderer::ReBuildDesLayout()
{
//Model  ubo (model view proj)
VkDescriptorSetLayoutBinding uboLayoutBinding{};
uboLayoutBinding.binding = 0;
uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
uboLayoutBinding.descriptorCount = 1;
uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;//指定在VERTEX阶段使用
uboLayoutBinding.pImmutableSamplers = nullptr;//跟预采样有关

//新描述符 Texture
VkDescriptorSetLayoutBinding samplerLayoutBinding{};
samplerLayoutBinding.binding = 1;
samplerLayoutBinding.descriptorCount = 1;
samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
samplerLayoutBinding.pImmutableSamplers = nullptr;
samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;//指定在Fragment阶段使用

//ViewPos
VkDescriptorSetLayoutBinding viewposLayoutBinding{};
viewposLayoutBinding.binding = 2;
viewposLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
viewposLayoutBinding.descriptorCount = 1;
viewposLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;//指定在Fragment阶段使用
viewposLayoutBinding.pImmutableSamplers = nullptr;//跟预采样有关

//绑定描述符
std::array<VkDescriptorSetLayoutBinding, 3> bindings = {uboLayoutBinding, samplerLayoutBinding,
viewposLayoutBinding};
VkDescriptorSetLayoutCreateInfo layoutInfo{};
layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
layoutInfo.pBindings = bindings.data();

if (vkCreateDescriptorSetLayout(g_device, &layoutInfo, nullptr, &descriptorSetLayout_) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create descriptor set layout!");
}

}

void LightRenderer::RebuildDescriptPool()
{
std::array<VkDescriptorPoolSize, 3> poolSizes{};
//uniform
poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//texture
poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//Phone
poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

VkDescriptorPoolCreateInfo poolInfo{};
poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
poolInfo.pPoolSizes = poolSizes.data();
poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * 4;

if (vkCreateDescriptorPool(g_device, &poolInfo, nullptr, &descriptorPool_) != VK_SUCCESS) {
    SG_CORE_ERROR("failed to create descriptor pool!");
}
}

//
void LightRenderer::RebuildDescriptorSets(std::vector<Texture2D*>& texs)
{
std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout_);
VkDescriptorSetAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
allocInfo.descriptorPool = descriptorPool_;
allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
allocInfo.pSetLayouts = layouts.data();
descriptorSets_.resize(MAX_FRAMES_IN_FLIGHT);
VkResult err;
err = vkAllocateDescriptorSets(g_device, &allocInfo, descriptorSets_.data());
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

std::array<VkWriteDescriptorSet,3> descriptorWrites{};
descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
descriptorWrites[0].dstSet = descriptorSets_[i];
descriptorWrites[0].dstBinding = 0;
descriptorWrites[0].dstArrayElement = 0;
descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
descriptorWrites[0].descriptorCount = 1;
descriptorWrites[0].pBufferInfo = &bufferInfo;
descriptorWrites[0].pImageInfo = nullptr; // Optional
descriptorWrites[0].pTexelBufferView = nullptr; // Optional

descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
descriptorWrites[1].dstSet = descriptorSets_[i];
descriptorWrites[1].dstBinding = 1;
descriptorWrites[1].dstArrayElement = 0;
descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
descriptorWrites[1].descriptorCount = static_cast<uint32_t>(imageInfos.size());
descriptorWrites[1].pImageInfo = imageInfos.data();

descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
descriptorWrites[2].dstSet = descriptorSets_[i];
descriptorWrites[2].dstBinding = 2;
descriptorWrites[2].dstArrayElement = 0;
descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
descriptorWrites[2].descriptorCount = 1;
descriptorWrites[2].pBufferInfo = &viewposbufferInfo;
descriptorWrites[2].pImageInfo = nullptr; // Optional
descriptorWrites[2].pTexelBufferView = nullptr; // Optional

vkUpdateDescriptorSets(g_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

}
}

void LightRenderer::RebuildUniform()
{
VkDeviceSize bufferSize_obj = sizeof(UniformBufferObject);

obj_uniform_->uniformBuffers_.resize(MAX_FRAMES_IN_FLIGHT);
obj_uniform_->uniformBuffersMemory_.resize(MAX_FRAMES_IN_FLIGHT);
obj_uniform_->uniformBuffersMapped_.resize(MAX_FRAMES_IN_FLIGHT);


VkDeviceSize bufferSize_light = sizeof(LightConstans);

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
