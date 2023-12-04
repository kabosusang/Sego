#pragma once
//定义UBO
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Log/Log.h"
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