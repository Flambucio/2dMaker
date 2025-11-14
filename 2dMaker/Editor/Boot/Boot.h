#pragma once
#include "Windows/Select.h"

namespace D2Maker
{
    namespace GUI
    {
        enum class ProjectSelectedFlag
        {
            NONE = -1,
            SELECTED,
        };

        class BootWindow
        {
        public:
            GLFWwindow* window = nullptr;
            ProjectSelectedFlag prjFlag = ProjectSelectedFlag::NONE;

            BootWindow();
            void RunBoot();
            bool BootWindowShouldClose();

            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
            {
                glViewport(0, 0, width, height);
            }
        };
    }
}