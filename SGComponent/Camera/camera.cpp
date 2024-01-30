#include "pch.h"

#include "SGComponent/Camera/camera.h"
#include "imgui.h"

//lookat
//摄像机位置
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//上轴
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


//摄像机方向
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);

//右向量
glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间


bool FPSmode = false;
bool GetcameraInput(KeyDownEvent& e)
{
    float cameraSpeed = 10.f * deltaTime; //adjust accordingly
    
    if (e.GetEventType() == EventType::KeyPressed)
    {
       int code = e.GetKeyCode();
        switch(code)
        {
            case GLFW_KEY_W:
            cameraPos += cameraSpeed * cameraFront;
            break;
            case GLFW_KEY_S:
            cameraPos -= cameraSpeed * cameraFront;
            break;
            case GLFW_KEY_A:
            cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
            break;
            case GLFW_KEY_D:
            {
            cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
            SG_CORE_INFO("DDDD");

            break;
            }
        
        }

        if(e.GetKeyCode() == GLFW_KEY_F)
            {
                FPSmode = !FPSmode;
            }
    }


    return true;
}

bool GetcameraRepeateInput(KeyDownRepeate& e)
{
    float cameraSpeed = 20.f * deltaTime; //adjust accordingly
    
    if (e.GetEventType() == EventType::KeyDownRepeate)
    {
       int code = e.GetKeyCode();
        switch(code)
        {
            case GLFW_KEY_W:
            transform_camera->add_position(cameraSpeed * cameraFront);
            break;
            case GLFW_KEY_S:
            transform_camera->add_position(-(cameraSpeed * cameraFront));
            break;
            case GLFW_KEY_A:
            transform_camera->add_position(-(glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed));
            break;
            case GLFW_KEY_D:
            {
             transform_camera->add_position((glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed));
            break;
            }
            

        
        }
    }

    return true;
}

//鼠标控制
//捕捉光标
//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
float lastX = 1800.f/2.0f, lastY = 1200.f/2.0f; //光标位置
float yaw = -90.0f; //yaw   ← →
float pitch = 0.0f;//pitch ↑ ↓
bool firstMouse = true;


//鼠标中键
bool middleButtonPressed = false;
float sceneMovementSpeed = 20.0f; // 场景移动速度，单位为像素/秒
float middleButton_lastX = 1280.f/2.0f, middleButton_lastY = 720.f/2.0f; //光标位置

bool GetCursorInput(MouseMoveEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    if(e.GetEventType() == EventType::MouseMoved && FPSmode) 
    {

        if(firstMouse) // 这个bool变量初始时是设定为true的
        {
            lastX = e.GetMouseX();
            lastY = e.GetMouseY();
            firstMouse = false;
        }

        float xoffset = e.GetMouseX() - lastX; // xoffset x->oldx
        float yoffset = lastY - e.GetMouseY(); //这是反的 y坐标从底部到顶部一次增大

        lastX = e.GetMouseX();
        lastY = e.GetMouseY();

        float sensitivity = 0.1f; //灵敏度
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
        //add limited
        if(pitch > 89.0f)
        pitch =  89.0f;
        if(pitch < -89.0f)
        pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }

    if(e.GetEventType() == EventType::MouseMoved && middleButtonPressed)
    {   

       SG_INFO("MID DOWN");
        
    }
    return true;
}


//fov 
float fov = 45.0f;
bool GetCursorScrollInput(MouseScrollEvent& e)
{  
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) // 检查是否鼠标在ImGui窗口上
    {
        if(fov >= 1.0f && fov <= 90.0f)
            fov -= e.GetScrollY();
        if(fov <= 1.0f)
            fov = 1.0f;
        if(fov >= 90.0f)
            fov = 90.0f;
    }

  return true;
}

bool GetMouseDownInput(MouseButtonPressedEvent& e)
{  
   if(e.GetEventType() == EventType::MouseButtonPressed)
    {
        if(e.GetMouseCode() == GLFW_MOUSE_BUTTON_MIDDLE )
        {
            SG_INFO("Down MIDDLE");
            middleButtonPressed = true;
        }
        
        
    }
    return true;
} 
//MouseButtonReleasedEvent
bool GetMouseReleaseInput(MouseButtonReleasedEvent& e)
{
    if(e.GetMouseCode() == GLFW_MOUSE_BUTTON_MIDDLE )
    {
        SG_INFO("Released MIDDLE");
        middleButtonPressed = false;
    }

}


#include <rttr/registration>
#include "SGComponent/game_object.h"

using namespace rttr;
RTTR_REGISTRATION//注册反射
{
    registration::class_<Camera>("Camera")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void Camera::SetView(const glm::vec3 &camerForward, const glm::vec3 &camerUp)
{
    auto transform = dynamic_cast<Transform*>(game_object()->GetComponent("Transform"));
    view_mat4_ = glm::lookAt(transform->position(), transform->position()+camerForward, cameraUp);
}

void Camera::SetProjection(float fovDegrees, float aspectRatio, float nearClip, float farClip)
{
    projection_mat4_ = glm::perspective(glm::radians(fovDegrees),aspectRatio,nearClip,farClip);
}


void Camera::Clear()
{
    
}

Camera* camera;
Transform* transform_camera;


