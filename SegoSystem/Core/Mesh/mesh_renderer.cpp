#include "Core/Mesh/mesh_renderer.h"
#include "SGComponent/game_object.h"
#include "SGComponent/transform.h"

#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <rttr/registration>


using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<MeshRenderer>("MeshRenderer")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr)
            .method("SetMaterial", &MeshRenderer::SetMaterial)
            .method("SetMeshFilter", &MeshRenderer::SetMeshFilter);

}


void MeshRenderer::SetMaterial(Material *material)
{
    material_=material;
}

void MeshRenderer::SetMeshFilter(MeshFilter *mesh_filter)
{
    mesh_filter_ = mesh_filter;
}

void MeshRenderer::Render()
{
    //主动获取Transform组件, 计算mvp
    auto component_transform = game_object()->GetComponent("Transform");
    auto transform = dynamic_cast<Transform*>(component_transform);
    if(!transform) {return;}

    glm::mat4 trans = glm::translate(transform->position());
    auto rotation = transform->rotation();
    glm::mat4 eulerAngleYXZ = glm::eulerAngleXYZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
    glm::mat4 scale = glm::scale(transform->scale()); //缩放;
    glm::mat4 model = trans*scale*eulerAngleYXZ;
    model_ = model;


    //主动获取 MeshFilter组件
    auto component_meshfilter = game_object()->GetComponent("MeshFilter");
    auto mesh_filter = dynamic_cast<MeshFilter*>(component_meshfilter);
    if(!mesh_filter) {return;}
    

}
