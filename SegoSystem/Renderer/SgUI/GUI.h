#pragma once
#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// Object 窗口
#define OBJECT_WDITH 400
#define OBJECT_HEIGHT 720
#define TITLE_HEIGHT 35

extern bool initnumber ;
extern bool Windowfollow ;
extern bool FollowbuttonState ;
extern bool FpsbuttonState ;
extern std::string WINDOW_FOLLOW ;
extern std::string FPS_MODE;
extern bool FPSmode;

class SGUI_Imgui
{
public:
SGUI_Imgui(const char* name,uint32_t id):UI_name_(name),UI_id_(id){}
void SetWindowPos(int x,int y){xpos = x,ypos = y;}
void SetWindowSize(int w,int h){width = w,height = h;};
protected:
std::string UI_name_;
uint32_t UI_id_;
//Main Window Attribution
int width,height;
int xpos,ypos;

};

class Object_Attr : public SGUI_Imgui
{
public:
Object_Attr(const char* name,uint32_t id);
void Run_UI();

private:
std::vector<int> DragList;
};


class App_Attr : public SGUI_Imgui
{
public:
App_Attr(const char* name,uint32_t id);
void Run_UI();



};







