#pragma once
#include "SGComponent/component.h"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "Shader/Uniform.h"
//Pipeline
#include "VulkanPipeLine/GraphicsPipelineManager.h"
#include "Descript.hpp"

class Material;
class MeshFilter;
class Texture2D;

class LightRenderer: public Component
{
public:
~LightRenderer();
LightRenderer();

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
void RebuildUniform();

//GetSampler
VkSampler& GetSampler();
VkImageView& GetImageview();


private:
Material* material_;
glm::mat4 Model_mat4_;
////////////////////////////VulkanData//////////////////////////////////
//Uniform
Uniform* obj_uniform_;
Uniform* light_uniform_;

//Descriptor
Sego::Vulkan::DescriptorManager* descriptorManager_;

//VkPiepline
std::vector<GraphicsPipelineManager> Vk_Pipeline_;

};

extern std::vector<LightRenderer*> Light_renderer;






