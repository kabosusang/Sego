#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//关节
struct Joint
{
    glm::mat4 m_BindPose_;
    const char* m_name_;
    uint8_t m_iParent_;
};

//skeleton
struct skeleton{
    uint32_t m_jointCount;//关节数
    Joint* m_aJoint;//关节数组
};

//关节姿势
struct JointPose
{
    glm::quat m_rot; //相对父关节朝向
    glm::vec3 m_trans; //在父关节的坐标
    glm::vec3 m_scale;  // 相对父关节的缩放
};

//蒙皮动画顶点
struct SkinnedVertex{
    float m_position[3]; //顶点位置(x,y,z)
    float m_normal[3]; //顶点法向量
    float m_u,m_v; //纹理坐标
    uint8_t m_jointIndex[4]; //关节的索引
    float m_jointWeight[4]; // 关节权重
};








