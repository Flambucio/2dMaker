#pragma once
#include "../../Editor/Main/Editor.h"
namespace D2Maker 
{

    


    static class Window
    {
    private:
        static GLFWwindow* window;
        static GLFWwindow* guiWindow;
        static int m_Width;
        static int m_Height;
        static GUI::Editor editor;
        static bool fullscreen;
        static int windowedX;
        static int windowedY;
        inline static ImGuiContext* mainImGuiContext = nullptr;
    public:


        static void Initialize();
        static inline void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        inline static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            glfwMakeContextCurrent(window);
            glViewport(0, 0, width, height);
        }
        inline static bool FileExists(const std::string& path)
        {
            std::ifstream file(path);
            return file.good();
        }
        static void ToggleFullscreen();
        inline static GLFWwindow* GetMainWindow()
        {
            return window;
        }
        static GLFWwindow* ConstructPreviewWindow();
        static void RunWindow();
        static void Destruct();

        

    };


    

    
}
