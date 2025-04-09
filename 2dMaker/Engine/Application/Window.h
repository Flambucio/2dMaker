#pragma once
#include "../common/Core.h"
namespace D2Maker 
{


    static class Window
    {
        static GLFWwindow* window;
    public:
        static void Initialize()
        {
            if (!glfwInit())
            {
                //throw error
                ERROR("failed to init glfw");
                return;
            }

            window = glfwCreateWindow(1280, 720, "game", NULL, NULL);
            if (!window)
            {
                ERROR("failed to create window");
                
                glfwTerminate();
                return;
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwMakeContextCurrent(window);
            GLenum err = glewInit();
            if (err!=GLEW_OK)
            {
                ERROR("failed to init glew");
                ERROR((const char*)glewGetErrorString(err));
                glfwTerminate();
                return;
            }

        }


        static void RunWindow()
        {
            while (!glfwWindowShouldClose(window))
            {
                glClear(GL_COLOR_BUFFER_BIT);
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }


        static void Destruct()
        {
            glfwTerminate();
            
        }

    };

    
}
