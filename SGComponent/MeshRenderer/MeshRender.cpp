#include "pch.h"
#include "MeshRenderer/MeshRender.h"
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


std::vector<MeshRenderer*> mesh_renderer;

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<MeshRenderer>("MeshRenderer")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void MeshRenderer::SetMaterial(Material* material) {
    material_=material;
    SG_CORE_INFO("Texture Name : {0}",material_->textures_.data()->first.c_str());

    std::vector<Texture2D*> texture2d;

    // 遍历源向量并提取 Texture2D* 添加到目标向量中
    for (const auto& pair : material_->textures_) {
        texture2d.push_back(pair.second);
    }


    //主动获取 MeshFilter 组件
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    if(!mesh_filter){
        return;
    }

    SG_CRes::SGvk_CreateDescriptorSets(
        app_device->descriptorSetLayout,
        app_device->descriptorPool,
        Obj_DescriptorSets_,
        texture2d,
        mesh_filter->Obj_uniformBuffers_,
        mesh_filter->Obj_uniformBuffersMemory_,
        mesh_filter->Obj_uniformBuffersMapped_
    );
  
}

MeshRenderer::MeshRenderer()
{

}
#include "VK_Global_Data.h"
MeshRenderer::~MeshRenderer()
{
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    //Models
    vkDestroyBuffer(g_device,mesh_filter->indexBuffer,nullptr);
    vkFreeMemory(g_device, mesh_filter->indexBufferMemory , nullptr);

    vkDestroyBuffer(g_device,mesh_filter->vertexBuffer, nullptr);//清理缓冲区
    vkFreeMemory(g_device, mesh_filter->vertexBufferMemory, nullptr);


    delete material_;
}

void MeshRenderer::Render(VkCommandBuffer& commandBuffer,uint32_t imageIndex)
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
    // 绑定顶点缓冲区
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mesh_filter->vertexBuffer ,offsets);
    // 绑定描述符集
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
    app_device->pipelineLayout, 0, 1, &Obj_DescriptorSets_[app_device->currentFrame] ,0, nullptr);
    
    // 绑定索引缓冲区并绘制模型
    vkCmdBindIndexBuffer(commandBuffer, mesh_filter->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh_filter->mesh()->indices32.size()), 1, 0, 0, 0);

}

std::vector<void *>& MeshRenderer::GetuniformBuffersMapped()
{
    //主动获取 MeshFilter 组件
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
   
    return mesh_filter->Obj_uniformBuffersMapped_;
}



