#include "SGComponent/component.h"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

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

private:
Material* material_;

glm::mat4 Model_mat4_;

////////////////////////////VulkanData//////////////////////////////////

//Descriptor
std::vector<VkDescriptorSet> Obj_DescriptorSets_;


};

extern std::vector<MeshRenderer*> mesh_renderer;






