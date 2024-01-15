#include "GUI.h"


bool Windowfollow = true;
bool FollowbuttonState = false;
bool FpsbuttonState = false;

std::string WINDOW_FOLLOW = u8"窗口跟随 : 未开启";
std::string FPS_MODE= u8"FPS:未开启";

///
///
///
///
//Object UI

Object_Attr::Object_Attr(const char *name, uint32_t id):SGUI_Imgui(name,id)
{
    
}
#include "Object/object.h"


glm::vec3 transform = glm::vec3(0.0f);
glm::ivec3 rotation = glm::ivec3(0);
glm::vec3 scale = glm::vec3(1.0f);
void Object_Attr::Run_UI()
{
//接收拖拽窗口
//ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + mainWindowSize.x, mainWindowPos.y), ImGuiCond_FirstUseEver);
  
    ImGui::Begin(u8"Object");
    
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
   
    
    if (!ImGui::IsAnyItemActive())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
    }

    ImGui::End();


    transform_obj[1]->set_position(transform);
    transform_obj[1]->set_rotation((glm::vec3)rotation);
    transform_obj[1]->set_scale(scale);

}
///
///
///
///
///
// SCene UI
App_Attr::App_Attr(const char *name, uint32_t id):SGUI_Imgui(name,id)
{

}


void App_Attr::Run_UI()
{

    std::string Text = "Hello Imgui";
    
    //Object 窗口
    ImGui::Begin(u8"Scene");
    
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

    if(ImGui::Button("ChangeModel"))
    {
        
    }

    ImGui::End();


}
