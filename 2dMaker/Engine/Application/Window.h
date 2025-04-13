#pragma once
#include "../common/Core.h"
#include "../ECS/Entity.h"
#include "../Graphics/GL/Shaders.h"
#include "../Graphics/GL/IndexBuffer.h"
#include "../Graphics/GL/VertexArray.h"
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
            //GRAPHICS
            float positions[] = {
                    -0.5f,-0.5f,
                     0.5f,-0.5f,
                     0.5f, 0.5f,
                    -0.5f, 0.5f,
            };

            unsigned int indices[] = {
                0,1,2,
                2,3,0
            };

            GLuint vao;


            unsigned int buffer;
            VertexArray va;
            VertexBuffer vb(positions, 4 * 2 * sizeof(float));
            VertexBufferLayout layout;
            layout.Push<float>(2);
            va.AddBuffer(vb, layout);
           

            IndexBuffer ib(indices, 6);

            Shaders shaderprogram;
            shaderprogram.Bind();

            //ECS
            EntityManager em;
            Entity entity1 = em.createEntity();
            Entity entity2 = em.createEntity();
            em.addComponent<Transform>(entity1, 1, 1, 100, 100, 0);//trace
            em.addComponent<Collider>(entity2);//warning
            em.addComponent<Collider>(entity1);//trace
            float r = 0.0f;
            float increment = 0.05f; 
            while (!glfwWindowShouldClose(window))
            {
                glClear(GL_COLOR_BUFFER_BIT);

                va.Bind();
                ib.Bind();
                

                shaderprogram.SetUniform4f("u_Color",r, 0.3f, 0.8f, 1.0f);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                if (r > 1.0f)
                {
                    increment = -0.05f;
                }
                else if (r < -1.0f)
                {
                    increment = 0.05f;
                }
                
                r += increment;

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
