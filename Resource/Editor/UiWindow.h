#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Editor
{
    class UiWindow final
    {
        public:
        inline static void Draw()
        {
            DrawMenu();
            DrawInspector();
            DrawProject();
            DrawHierarchy();
            DrawConsole();
            DrawViewport();
        }

        private:
        static void DrawMenu();
        static void DrawInspector();
        static void DrawProject();
        static void DrawHierarchy();
        static void DrawConsole();
        static void DrawViewport();


        static std::vector<int> DragList;

       
    };

};