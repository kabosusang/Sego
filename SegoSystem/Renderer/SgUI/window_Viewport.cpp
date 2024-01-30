#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Editor/UiWindow.h"

namespace Editor
{
	void UiWindow::DrawViewport()
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

        if (ImGui::Begin("Scene", NULL))
        {
            ImGui::Image(ImGuiLogic::descriptor->set, ImGui::GetContentRegionAvail());

            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Viewport Size: %.fx%.f", size.x, size.y);
        }

        ImGui::PopStyleColor();



    }

};


