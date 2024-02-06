#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>

#include "Editor/UiWindow.h"
#include "Renderer/Vulkan/Vk_Device_Init.h"
#include "VK_Global_Data.h"
#include "Sg_UI.h"

namespace Editor
{

	void UiWindow::DrawViewport()
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        //Copy to viewportimage
        auto textureID = ImGui_ImplVulkan_AddTexture(UiContext->viewportSampler,
        UiContext->viewportImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        
        if (ImGui::Begin("Viewport")) {
        ImGui::Image(textureID, ImGui::GetContentRegionAvail());
        }
        ImGui::End();
        ImGui::PopStyleColor();

    }

};


