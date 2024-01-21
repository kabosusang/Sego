#include <glm/gtc/matrix_transform.hpp>

#include "CameraComponent.h"
void CmaeraComponent::SetOrientation(const glm::quat &qRot)
{
}

glm::vec3 CmaeraComponent::GetEulerAngles() const
{
    return glm::vec3();
}

void CmaeraComponent::SetEulerAngles(const glm::vec3 &vEulerAngles)
{
}



void CmaeraComponent::LookAtLH(const glm::vec3 &pos, const glm::vec3 &target, const glm::vec3 &worldUp)
{
    this->m_vPos = pos;
    glm::mat4 mat = glm::lookAtLH(pos,target,worldUp);
    
    //FQuaternion qRot = ToQuaternion(FMath::TransposeMatrix3(mat));

}

void CmaeraComponent::PerspectiveLH(float fFovY, float fAspect, float fNearZ, float fFarZ)
{
}

void CmaeraComponent::Strafe(float dis)
{
}

void CmaeraComponent::Walk(float dis)
{
}

void CmaeraComponent::Pitch(float angle)
{
}

void CmaeraComponent::Yaw(float angle)
{
}

void CmaeraComponent::Roll(float angle)
{
}

void CmaeraComponent::Rotate(const glm::vec3 &axis, float angle)
{
}

void CmaeraComponent::Rotate(const glm::quat &qRot)
{
}

void CmaeraComponent::UpdateViewMatrix()
{
}

void CmaeraComponent::UpdateProjectionMatrix()
{
}
