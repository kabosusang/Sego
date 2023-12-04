#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// instance
class Phydevice
{
public:
Phydevice() 
{
glfwGetMonitors(&monitorCount);
primarymode = glfwGetVideoMode(primaryMonitor[0]);
DeviceWidth = primarymode->width;
DeviceHeight = primarymode->height;
}
Phydevice(const Phydevice&) = delete;
Phydevice(const Phydevice&&) = delete;
Phydevice& operator=(const Phydevice&) = delete;
public: 
static Phydevice& GetInstance()
{
    static Phydevice s;
    return s;
}

int GetMonitorW(){return DeviceWidth;}
int GetMonitorH(){return DeviceHeight;}

private:
//主屏幕
GLFWmonitor** primaryMonitor;
//主屏幕模式
const GLFWvidmode* primarymode;
//屏幕分辨率
int DeviceWidth;
int DeviceHeight;
//屏幕数量
int monitorCount;
};


struct MonitorAttribute
{
    MonitorAttribute():MonitorWidth(0),MonitorHeight(0),MonitorIndex(0) {}
    int MonitorWidth;
    int MonitorHeight;
    int MonitorIndex;
};


























