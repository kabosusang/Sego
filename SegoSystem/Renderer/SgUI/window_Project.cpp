#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "SGComponent/MeshRenderer/MeshRender.h"
#include "Editor/UiWindow.h"

namespace Editor
{
	void UiWindow::DrawProject()
    {   
        std::string Text = "Hello Imgui";
        //Object 窗口
        ImGui::Begin(u8"Project");
        
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
        
        ImGui::Checkbox("OultLine", &mesh_renderer[0]->OutLine);

        ImGui::End();

    }

};

 
 

 
 
 