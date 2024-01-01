#pragma once
//定义UBO

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Log/Log.h"
#include <vector>
//模型UBO
struct UniformBufferObject{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
bool GetcameraInput(KeyDownEvent& e); //key
bool GetcameraRepeateInput(KeyDownRepeate& e);

//cursor function
bool GetCursorInput(MouseMoveEvent& e);
bool GetCursorScrollInput(MouseScrollEvent& e);

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront ;
extern glm::vec3 cameraUp ;

extern glm::vec3 cameraTarget;
extern glm::vec3 cameraDirection ;

extern glm::vec3 up;
extern glm::vec3 cameraRight ;

extern float deltaTime ;
extern float lastFrame;

//cursor
extern float lastX ;
extern float lastY ;

//scroll
extern float fov;
extern bool FPSmode;

#include "SGComponent/component.h"
class Camera: public Component
{
public:
//camerForward 相机超前方向
//camerUp 相机朝上方向
void SetView(const glm::vec3& camerForward,const glm::vec3& camerUp);

//设置相机视野
/// param fovDegrees   相机视野 可视角度
/// param aspectRatio  宽高比
/// param nearClip 近裁剪面
/// param farClip  远裁剪面
void SetProjection(float fovDegrees,float aspectRatio,float nearClip,float farClip);



/// 刷帧清屏
void Clear();


glm::mat4& view_mat4(){return view_mat4_;}
glm::mat4& projection_mat4(){return projection_mat4_;}


private:
    glm::mat4 view_mat4_;//指定相机坐标和朝向
    glm::mat4 projection_mat4_;//指定相机范围
    glm::vec4 clear_color; //清屏颜色
 
};
#include "SGComponent/transform.h"
extern Camera* camera;
extern Transform* transform_camera;




