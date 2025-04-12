#pragma once
#include "../common/Core.h"
#include "../ECS/Entity.h"
#include "../Graphics/Shaders.h"
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
        



        static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
        {

        }

        static void RunWindow()
        {
            unsigned int buffer;
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
            glEnableVertexAttribArray(0);
            float vertices[] = {
                -0.5f,-0.5f,
                 0.0f, 0.5f,
                 0.5f,-0.5f
            };
            glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);
            Shaders shaderprogram;
            GLuint shader=shaderprogram.CreateShaderProgram();
            glUseProgram(shader);
            EntityManager em;
            Entity entity1 = em.createEntity();
            Entity entity2 = em.createEntity();
            em.addComponent<Transform>(entity1, 1, 1, 100, 100, 0);//trace
            em.addComponent<Collider>(entity2);//warning
            em.addComponent<Collider>(entity1);//trace
            while (!glfwWindowShouldClose(window))
            {
                glClear(GL_COLOR_BUFFER_BIT);

                glDrawArrays(GL_TRIANGLES,0,3);
                
                


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
