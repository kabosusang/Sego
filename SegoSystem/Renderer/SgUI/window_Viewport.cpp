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
       
        //Copy to viewportimage
        UiContext->viewportDescriptorSet = ImGui_ImplVulkan_AddTexture(UiContext->viewportSampler,
        UiContext->viewportImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        if(UiContext->viewportDescriptorSet)
        { 
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            if (ImGui::Begin("Viewport")) {
            ImGui::Image(UiContext->viewportDescriptorSet, ImGui::GetContentRegionAvail());
            }
            ImGui::End();
            ImGui::PopStyleColor();
        }
    }

};


