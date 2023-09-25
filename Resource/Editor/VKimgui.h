// Dear ImGui: standalone example application for Glfw + Vulkan
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// Important note to the reader who wish to integrate imgui_impl_vulkan.cpp/.h in their own engine/app.
// - Common ImGui_ImplVulkan_XXX functions and structures are used to interface with imgui_impl_vulkan.cpp/.h.
//   You will use those if you want to use this rendering backend in your engine/app.
// - Helper ImGui_ImplVulkanH_XXX functions and structures are only used by this example (main.cpp) and by
//   the backend itself (imgui_impl_vulkan.cpp), but should PROBABLY NOT be used by your own engine/app code.
// Read comments in imgui_impl_vulkan.h.
#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
//#include <vulkan/vulkan_beta.h>

#include <iostream>



// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif




//imgui Function
void glfw_error_callback(int error, const char* description);
void check_vk_result(VkResult err);
VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);

bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();

void SetupVulkan(ImVector<const char*> instance_extensions);
void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
void CleanupVulkan();
void CleanupVulkanWindow();
void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
void FramePresent(ImGui_ImplVulkanH_Window* wd);

//Render Imgui
void Init_Imgui(GLFWwindow* window);
void Cleanup_imgui();

// extern Data
extern VkAllocationCallbacks*   g_Allocator ;
extern VkInstance               g_Instance;
extern VkPhysicalDevice         g_PhysicalDevice;
extern VkDevice                 g_Device ;
extern uint32_t                 g_QueueFamily ;
extern VkQueue                  g_Queue ;
extern VkDebugReportCallbackEXT g_DebugReport ;
extern VkPipelineCache          g_PipelineCache ;
extern VkDescriptorPool         g_DescriptorPool ;

extern ImGui_ImplVulkanH_Window g_MainWindowData;
extern int                      g_MinImageCount ;
extern bool                     g_SwapChainRebuild ;

//imgui 


class ImguiLayer
{
private:
ImGui_ImplVulkan_InitInfo init_info;
public:
void Init_Imgui(GLFWwindow* window);

ImguiLayer& operator=(const ImguiLayer&) = delete;

ImguiLayer(const ImGui_ImplVulkan_InitInfo& IGVuIn) { init_info = IGVuIn;}

};