#pragma once
#include "SGComponent/component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class CmaeraComponent : public Component
{
    glm::vec3 m_vPos; //位置
    glm::quat m_qRot; //四元素代表的xyz轴的旋转

    float m_fNearZ; //近裁剪面距离
    float m_fFarZ; //远裁剪面距离
    float m_fAspect; //宽高比例
    float m_fFovY; //视野

    glm::mat4 m_mat4View; //View变换矩阵
    glm::mat4 m_mat4Projection; //Projection变换矩阵

public:
    const glm::vec3& GetPos() const {return this->m_vPos;}
    void SetPos(float x,float y,float z) {SetPos(glm::vec3(x,y,z));}
    void SetPos(const glm::vec3& v)
    {
        this->m_vPos = v;
    }

    const glm::quat& GetOrientation() const {return m_qRot;}
    void SetOrientation(const glm::quat& qRot);

    glm::vec3 GetEulerAngles() const ; //获取欧拉角度
    void SetEulerAngles(const glm::vec3& vEulerAngles);////通过欧拉角度设置四元素

public:
    //构造摄像机
    void LookAtLH(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& vUp);
    void PerspectiveLH(float fFovY, float fAspect, float fNearZ, float fFarZ); //构造Projection矩阵

    void Strafe(float dis); //沿Right方向移动摄像机
    void Walk(float dis); //沿Dir方向移动摄像机

    void Pitch(float angle); //Rotate with X Axis
    void Yaw(float angle); //Rotate with Y Axis
    void Roll(float angle); //Rotate with Z Axis

    void Rotate(const glm::vec3& axis, float angle); //绕轴axis旋转angle角度
    void Rotate(const glm::quat& qRot); //按四元素旋转

    void UpdateViewMatrix(); //更新摄像机View矩阵
    void UpdateProjectionMatrix(); //更新摄像机Projection矩阵




};