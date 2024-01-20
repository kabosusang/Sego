#pragma once
#include "SGComponent/component.h"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

//Pipeline
#include "VulkanPipeLine/GraphicsPipelineManager.h"

class Material;
class MeshFilter;
class Texture2D;

class MeshRenderer: public Component
{
public:
MeshRenderer();
~MeshRenderer();

void SetMaterial(Material* material); //设置material
Material* material(){return material_;}
void Render(VkCommandBuffer& commandBuffer,uint32_t imageIndex); //渲染

inline glm::mat4& Model_mat4() { return Model_mat4_;}
std::vector<void*>& GetuniformBuffersMapped();

void RecreatePipeline();
void CleanPipeLine();
void SwitchOutLine(bool newStatus);

//OultLine
bool OutLine = false;

//blend
bool blend = false;

//instance Count
uint16_t instanceCount = 1;
private:
Material* material_;
glm::mat4 Model_mat4_;

////////////////////////////VulkanData//////////////////////////////////
//Descriptor
std::vector<VkDescriptorSet> Obj_DescriptorSets_;
//VkPiepline
std::vector<GraphicsPipelineManager> Vk_Pipeline_;


};

extern std::vector<MeshRenderer*> mesh_renderer;






