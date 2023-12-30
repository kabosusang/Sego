#pragma once

#include "core.h"
#include "Events/Event.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stb_image.h>
#include <functional>


class SegoWindow
{
protected:
using EventCallbackFn = std::function<void(Event&)>;

struct WindowData
{
	std::string Title;
	unsigned int Width, Height;
	bool VSync;
	EventCallbackFn EventCallback;
};

public:
void Init();

SegoWindow(GLFWwindow* gwindow,int w,int h) : GLwindow(gwindow), Window_Width_(w),Window_Height_(h)
{
GLFWimage images[1]; 
images[0].pixels = stbi_load("../SegoSystem/image/Sego.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
glfwSetWindowIcon(gwindow, 1, images); 
stbi_image_free(images[0].pixels);

}
~SegoWindow() { }

//Window attributes
inline void SetEventCallback(const EventCallbackFn& callback)
{
	m_Data.EventCallback = callback;
}

inline void DestorySegowindow() {glfwDestroyWindow(GLwindow);glfwTerminate();}
inline int GetHeight() { return Window_Height_;}
inline int GetWidth() { return Window_Width_;}
inline int GetXpos() {return Window_Posx_;}
inline int GetYpos() {return Window_Posy_;}

inline void SetWindowSize(int w,int h) {  Window_Width_ = w; Window_Height_ = h;}
inline void SetWindowPos(int x,int y) {  Window_Posx_ = x; Window_Posy_ = y;}

inline GLFWwindow* GetWindow() {return GLwindow ;}
private:
GLFWwindow* GLwindow;
//不包含标题栏的大小
int Window_Width_;
int Window_Height_;

//窗口位置
int Window_Posx_;
int Window_Posy_;


WindowData m_Data;
};



