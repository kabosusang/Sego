#pragma once

#include "core.h"

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class SegoWindow
{
public:
SegoWindow(GLFWwindow* gwindow,int w,int h) : GLwindow(gwindow), Window_Width(w),Window_Height(h){}
~SegoWindow() { }

inline void DestorySegowindow() {glfwDestroyWindow(GLwindow);glfwTerminate();}

inline int GetHeight() { return Window_Height;}
inline int GetWidth() { return Window_Width;}

inline GLFWwindow* GetWindow() {return GLwindow ;}
private:
GLFWwindow* GLwindow;
int Window_Width;
int Window_Height;
};



