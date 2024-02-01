#pragma once
#include "SGComponent/component.h"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "Shader/Uniform.h"
//Pipeline
#include "VulkanPipeLine/GraphicsPipelineManager.h"

class Material;
class MeshFilter;
class Texture2D;

class LightRenderer: public Component
{
public:
LightRenderer();
~LightRenderer();

void SetMaterial(Material* material); //设置material
Material* material(){return material_;}
void Render(VkCommandBuffer& commandBuffer,uint32_t imageIndex); //渲染

inline glm::mat4& Model_mat4() { return Model_mat4_;}
//uniform onj
std::vector<VkDeviceMemory>& GetuniformBufferMemory_obj();
std::vector<void*>& GetuniformBuffersMapped_obj();

//uniform light
std::vector<VkDeviceMemory>& GetuniformBufferMemory_light();
std::vector<void*>& GetuniformBuffersMapped_light();

void RecreatePipeline();
void CleanPipeLine();


//instance Count
uint16_t instanceCount = 1;



//Des
void ReBuildDesLayout();
void RebuildDescriptPool();
void RebuildDescriptorSets(std::vector<Texture2D*>& texs);
void RebuildUniform();



//GetSampler
VkSampler& GetSampler();
VkImageView& GetImageview();

//DescriptorSet
std::vector<VkDescriptorSet>  descriptorSets_;
private:
Material* material_;
glm::mat4 Model_mat4_;
////////////////////////////VulkanData//////////////////////////////////
//Uniform
Uniform* obj_uniform_;
Uniform* light_uniform_;


//Des Layout
VkDescriptorSetLayout descriptorSetLayout_; //描述绑定符
VkDescriptorPool descriptorPool_;

//VkPiepline
std::vector<GraphicsPipelineManager> Vk_Pipeline_;

};

extern std::vector<LightRenderer*> Light_renderer;






