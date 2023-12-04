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

bool initnumber = true;
bool Windowfollow = false;


bool FollowbuttonState = false;
bool FpsbuttonState = false;

std::string WINDOW_FOLLOW = u8"窗口跟随 : 未开启";
std::string FPS_MODE= u8"FPS:未开启";
extern bool FPSmode;


std::vector<int> DragList;
void DrawGUI(GLFWwindow* window)
{
    std::string Text = "Hello Imgui";
   
    //获取窗口位置
    int xpos,ypos;
    glfwGetWindowPos(window,&xpos,&ypos);
    //获取窗口大小
    int w,h;
    glfwGetWindowSize(window,&w,&h);
    //设置imgui的窗口位置
    if(initnumber)
    {
        ImGui::SetNextWindowSize(ImVec2(OBJECT_WDITH , OBJECT_HEIGHT+TITLE_HEIGHT));
    }
    //Object 窗口
    ImGui::Begin(u8"Object");
    if(initnumber)
    {
     ImGui::SetWindowPos(ImVec2(xpos + w  ,ypos - TITLE_HEIGHT));
     initnumber = false;
    }   
    if(Windowfollow)
    {
        ImGui::SetWindowPos(ImVec2(xpos + w  ,ypos - TITLE_HEIGHT));
    }
    if(ImGui::BeginCombo(u8"哈哈",Text.c_str()))
    {
        for (size_t i =0; i< 100 ; i++)
        {
            if(ImGui::Selectable(std::to_string(i).c_str()))
            {
                Text = std::to_string(i);
            }
        }
        ImGui::EndCombo();
    }
    if(ImGui::IsKeyDown(ImGuiKey_Q) && ImGui::IsItemHovered())
    {
        ImGui::Text(u8"按下了Q");
    }

    for(size_t i =0 ; i<5 ; i++)
    {
        ImGui::Button(std::to_string(i).c_str());
        if (i+1 < 5)
        {
            ImGui::SameLine();
        }
        if (ImGui::BeginDragDropSource()) //拖撤事件
        {
            ImGui::Text(std::string("Drag :").append(std::to_string(i)).c_str());

            ImGui::SetDragDropPayload("DragIndexButton",&i,sizeof(int));
            ImGui::EndDragDropSource();
        }
    
    }
    //窗口跟随
    
    if(ImGui::Button(WINDOW_FOLLOW.c_str()))
    {
        FollowbuttonState = !FollowbuttonState;
    }
    if(!FollowbuttonState)
    {
        WINDOW_FOLLOW = u8"窗口跟随 : 已开启";
        Windowfollow = true; 
    }
    else
    {
        WINDOW_FOLLOW = u8"窗口跟随 : 未开启";\
        Windowfollow = false;
    }
    ImGui::SameLine;
    //fps 视角
    if(ImGui::Button(FPS_MODE.c_str()))
    {
        FpsbuttonState = !FpsbuttonState;
    }
    if(FpsbuttonState)
    {
        FPS_MODE = u8"FPS:已开启";
        FPSmode = true;
    }
    else
    {
        FPS_MODE = u8"FPS:未开启";
        FPSmode = false;
    }

    ImGui::End();

   
    //接收拖拽窗口
    
    //ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + mainWindowSize.x, mainWindowPos.y), ImGuiCond_FirstUseEver);
   ImGui::SetNextWindowSize(ImVec2(OBJECT_WDITH , OBJECT_HEIGHT+TITLE_HEIGHT));
    ImGui::Begin("Drag Window");
    if(Windowfollow)
    {
        ImGui::SetWindowPos(ImVec2(xpos - OBJECT_WDITH  ,ypos - TITLE_HEIGHT));
    }
    ImGui::Text("Drag Target");
    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragIndexButton"))
        {
            DragList.push_back(*(const int*)payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
    for(size_t i =0 ; i < DragList.size() ; i++)
    {
        if(ImGui::Button(std::to_string(DragList.at(i)).c_str()))
        {
            DragList.erase(DragList.begin() + i);
        }
        if (i+1 < DragList.size())
        {
            ImGui::SameLine();
        }
      
    }

    ImGui::End();

    //ImGui::ShowDemoWindow();

}