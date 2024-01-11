#include "pch.h"
#include "MeshRenderer/MeshRender.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <rttr/registration>

#include "game_object.h"
#include "transform.h"
#include "MeshFilter/MeshFilter.h"
using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<MeshRenderer>("MeshRenderer")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void MeshRenderer::SetMaterial(Material* material) {
    material_=material;
}

MeshRenderer::MeshRenderer()
{

}
MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
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
glm::mat4 mvp = trans * scale * eulerAngleYXZ;

//主动获取 MeshFilter 组件
    auto component_mesh_filter=game_object()->GetComponent("MeshFilter");
    auto mesh_filter=dynamic_cast<MeshFilter*>(component_mesh_filter);
    if(!mesh_filter){
        return;
    }

    











}


