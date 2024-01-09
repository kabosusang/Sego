#include "Object/object.h"


std::vector<Transform*> transform_obj;
std::vector<SG_Object*> sg_obj;


#include <rttr/registration>
#include "SGComponent/game_object.h"
#include "object.h"

#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<SG_Object>("SG_Object")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void SG_Object::ChangeModelPosition()
{
auto transform = dynamic_cast<Transform*>(game_object()->GetComponent("Transform"));
auto rotation=transform->rotation();
glm::mat4 trans = glm::translate(transform->position());
glm::mat4 scale = glm::scale(transform->scale()); //缩放
glm::mat4 eulerAngleYXZ = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
Model_mat4_ = trans * scale * eulerAngleYXZ;
}
