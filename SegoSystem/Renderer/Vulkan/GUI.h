#pragma once
#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>

std::vector<int> DragList;
void DrawGUI()
{
    std::string Text = "Hello Imgui";
    ImGui::Begin(u8"左臂");
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

    ImGui::End();

   
    //接收拖拽
    //ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + mainWindowSize.x, mainWindowPos.y), ImGuiCond_FirstUseEver);
    ImGui::Begin("Drag Window");
    
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

    ImGui::ShowDemoWindow();

}