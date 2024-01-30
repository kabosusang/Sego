#include <imgui_impl_vulkan.h>
#include <vector>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Editor/UiWindow.h"



namespace Editor
{
    void UiWindow::DrawConsole()
	{
        ImGui::Begin(u8"Console");
        ImGui::Text("Hello Console");
        ImGui::End();
    }

}
