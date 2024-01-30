#include <glm/gtc/type_ptr.hpp>
#include "SGComponent/MeshRenderer/MeshRender.h"
#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Editor/UiWindow.h"

glm::vec3 transform = glm::vec3(0.0f);
glm::ivec3 rotation = glm::ivec3(0);
glm::vec3 scale = glm::vec3(1.0f);
#include "Object/object.h"
#include "Lights/LightConstans.h"

std::vector<int> Editor::UiWindow::DragList;
namespace Editor
{
    void UiWindow::DrawInspector()
	{
        //接收拖拽窗口
        //ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + mainWindowSize.x, mainWindowPos.y), ImGuiCond_FirstUseEver);
    
        ImGui::Begin(u8"Inspector");
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
        if (ImGui::CollapsingHeader(u8"变换"))
        {
            ImGui::SeparatorText("Transform");

            ImGui::Text("Avg fps: %.3f", ImGui::GetIO().Framerate);
            
            //Transform
            ImGui::Text(u8"Transform");
            ImGui::Text(u8"X");
            ImGui::SameLine(); 
            if(ImGui::DragFloat("##transform_x",&transform.x, 0.005f))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }
        
            ImGui::Text(u8"Y");
            ImGui::SameLine(); 
            if(ImGui::DragFloat("##transform_y",&transform.y, 0.005f))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }

            ImGui::Text(u8"Z");
            ImGui::SameLine(); 
            if(ImGui::DragFloat("##transform_z",&transform.z, 0.005f))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }
        
            //Rotation
            ImGui::Text(u8"Rotation");
            ImGui::Text(u8"X");
            ImGui::SameLine(); 
            if(ImGui::DragInt("##rotation_x",&rotation.x, 1, 0, 0,"%d °"))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }

            ImGui::Text(u8"Y");
            ImGui::SameLine(); 
            if(ImGui::DragInt("##rotation_y",&rotation.y, 1 ,0, 0,"%d °"))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }

            ImGui::Text(u8"Z");
            ImGui::SameLine(); 
            if(ImGui::DragInt("##rotation_z",&rotation.z, 1, 0, 0,"%d °"))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }
        
            //Scale
            ImGui::Text(u8"scale");
            ImGui::Text(u8"X");
            ImGui::SameLine(); 
            if(ImGui::DragFloat("##scale_x", &scale.x, 0.01f))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }
            
            ImGui::Text(u8"Y");
            ImGui::SameLine(); 
            if(ImGui::DragFloat("##scale_y", &scale.y, 0.01f))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }


            ImGui::Text(u8"Z");
            ImGui::SameLine(); 
            if(ImGui::DragFloat("##scale_z", &scale.z, 0.01f))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }
        }
        if (ImGui::CollapsingHeader(u8"光照"))
        {
            ImGui::SeparatorText("Phone");
            
            ImGui::ColorEdit3("diffuse", glm::value_ptr(phone.diffuse));
            ImGui::ColorEdit3("specular", glm::value_ptr(phone.specular));
        
            
            ImGui::DragFloat("shininess", &phone.shininess, 1.0f);
            ImGui::DragFloat("lightambient", &phone.lightambient, .05f);
            ImGui::DragFloat("lightspecular", &phone.lightspecular, 0.05f);
        /*
            alignas(16) glm::vec3 diffuse;
            alignas(16) glm::vec3 specular;
            alignas(4)  float shininess = 32;

            alignas(4) float lightambient = 0.1f; //环境光照
            alignas(4) float lightspecular = 0.5f; //镜面光分量
        */
        }

    
        
        if (!ImGui::IsAnyItemActive())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
        }

        ImGui::End();


            transform_obj[3]->set_position(transform);
            transform_obj[3]->set_rotation((glm::vec3)rotation);
            transform_obj[3]->set_scale(scale);
        }


};


