#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>

#include "Editor/UiWindow.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "VK_Global_Data.h"

namespace Editor
{

	void UiWindow::DrawViewport()
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                //ImGui截胡
        ImGui::Begin("Viewport");
       
      
        ImGui::End();
        ImGui::PopStyleColor();


    }

};


