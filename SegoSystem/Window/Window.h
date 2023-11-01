#pragma once

#include "core.h"
#include "Events/Event.h"
#include "VKimgui.h"
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

SegoWindow(GLFWwindow* gwindow,int w,int h) : GLwindow(gwindow), Window_Width(w),Window_Height(h)
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
inline int GetHeight() { return Window_Height;}
inline int GetWidth() { return Window_Width;}
inline GLFWwindow* GetWindow() {return GLwindow ;}


private:
GLFWwindow* GLwindow;
int Window_Width;
int Window_Height;

WindowData m_Data;
};



