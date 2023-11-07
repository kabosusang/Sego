#include "camera/camera.h"


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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

bool GetcameraInput(KeyDownEvent& e)
{
    float cameraSpeed = 2.5f * deltaTime; //adjust accordingly
    
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
    }

    

    return true;
}


bool GetcameraRepeateInput(KeyDownRepeate& e)
{
    float cameraSpeed = 2.5f * deltaTime; //adjust accordingly
    
    if (e.GetEventType() == EventType::KeyDownRepeate)
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
    }

    

    return true;
}

//鼠标控制
//捕捉光标
//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
float lastX = 1280.f/2.0f, lastY = 720.f/2.0f; //光标位置
float yaw = -90.0f; //yaw   ← →
float pitch = 0.0f;//pitch ↑ ↓
bool firstMouse = true;

bool FPSmode = false;

//direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw)); // 译注：direction代表摄像机的前轴(Front)，这个前轴是和本文第一幅图片的第二个摄像机的方向向量是相反的
//direction.y = sin(glm::radians(pitch));
//direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

bool GetCursorInput(MouseMoveEvent& e)
{
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

        float sensitivity = 0.05f; //灵敏度
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

    
}


//fov 
float fov = 45.0f;
bool GetCursorScrollInput(MouseScrollEvent& e)
{
    

    if(fov >= 1.0f && fov <= 45.0f)
        fov -= e.GetScrollY();
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;


}