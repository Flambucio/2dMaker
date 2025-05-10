#pragma once
#include "../Filesystem/Filesystem.h"
namespace D2Maker 
{


    static class Window
    {
        static GLFWwindow* window;
        static int m_Width;
        static int m_Height;

    
    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
    public:


        static void Initialize()
        {
            //OPENGL
            if (!glfwInit())
            {
                //throw error
                ERROR("failed to init glfw");
                return;
            }


            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            window = glfwCreateWindow(DEFAULT_WIN_W, DEFAULT_WIN_H, CAPTION, NULL, NULL);
            if (!window)
            {
                ERROR("failed to create window");

                glfwTerminate();
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSetWindowAspectRatio(window, 16, 9);
            GLenum err = glewInit();
            if (err!=GLEW_OK)
            {
                ERROR("failed to init glew");
                ERROR((const char*)glewGetErrorString(err));
                glfwTerminate();
                return;
            }
            //OPENAL
            ALCdevice* device = alcOpenDevice(nullptr);
            if (!device)
            {
                ERROR("failed to open OpenAL device");
                return;
            }
            ALCcontext* context = alcCreateContext(device, nullptr);
            if (!context || alcMakeContextCurrent(context)==ALC_FALSE)
            {
                ERROR("failed to create or set OpenAL context");
                return;
            }

            TRACE("USING DEVICE=");
            TRACE(alcGetString(device, ALC_DEVICE_SPECIFIER))
            



        }
        

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                EventManager::PushEvent(static_cast<Keys>(key));
            }
            else if(action==GLFW_RELEASE)
            {
                EventManager::ReleaseKey(static_cast<Keys>(key));
            }
        }
        static bool FileExists(const std::string& path)
        {
            std::ifstream file(path);
            return file.good();
        }

        static void RunWindow()
        {
            TextureLoader::LoadTexture("erbucio", "Engine/Resources/TestAssets/image.png", 0);
            TextureLoader::LoadTexture("a", "Engine/Resources/TestAssets/gin.png", 0);
            AudioLoader::LoadAudio("erbuciaccio", "Engine/Resources/TestAssets/numayey.ogg");
            AudioLoader::LoadAudio("disco", "Engine/Resources/TestAssets/disco.ogg");
            FileSys::GetProjects();
            
            FileSys::SelectProject("Project1");
            FileSys::LoadScenes();
            TRACE("CURRENT PRJ"+FileSys::currentProject);
            PRINT_U_SET_STR(FileSys::projectNames);
            //SceneManager::AddScene("testscene");
            //Scene* scene =SceneManager::GetScene("testscene");


            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwSetKeyCallback(window, KeyCallback);
            /*SystemManager sm{window};
            EntityManager em;
            Entity entity1 = scene->em.createEntity("e1");
            Entity entity2 = scene->em.createEntity("e2");
            Entity entiti3 = scene->em.createEntity("e3");
            Entity entity4 = scene->em.createEntity("e4");
            Entity entity5 = scene->em.createEntity("e5");
            Entity entity6 = scene->em.createEntity("e6");
            int e1_width = 200;
            int e1_height = 200;
            //1
            scene->em.addComponent<Transform>(entity1, e1_width, e1_height, 600, 500, 0);//trace
            scene->em.addComponent<Collider>(entity1);//trace
            //em.addComponent<TextureComponent>(entity1,"erbucio",0);
            scene->em.addComponent<Velocity>(entity1, 0, 0,0);
            //em.addComponent<Camera>(entity1,true,false,CENTER_X-e1_width,CENTER_Y-e1_height/2);
         
            scene->em.addComponent<Timer>(entity1);
            scene->em.addComponent<Script>(entity1, "Projects/Script.txt");
            std::vector<std::string> names = { "erbucio","a"};
            scene->em.addComponent<Animation>(entity1, names,2,0);
            //2
            scene->em.addComponent<Transform>(entity2, 0, 0, 1600, 900,0);
            scene->em.addComponent<TextureComponent>(entity2, "erbucio", -1);
            //3
            scene->em.addComponent<Transform>(entiti3, 1200, 200, 600, 500, 45);
            scene->em.addComponent<Collider>(entiti3);
            scene->em.addComponent<TextureComponent>(entiti3, "erbucio", 0);
            //4
            scene->em.addComponent<Transform>(entity4, 0, 850, 1600, 100,0);
            scene->em.addComponent<Collider>(entity4);
            scene->em.addComponent<TextureComponent>(entity4, "erbucio", 0);
            //5
            scene->em.addComponent<Script>(entity5, "Projects/Script2.txt");

            scene->em.addComponent<Transform>(entity6, 1000, 700, 100, 100, 0);
            scene->em.addComponent<Velocity>(entity6, 0, 0,0);
            scene->em.addComponent<Collider>(entity6);
            scene->em.addComponent<TextureComponent>(entity6, "a", 2);
            scene->em.addComponent<Follow>(entity6, 10, "e1");
            */
            
            SceneManager::SelectScene("testscene");
            if(SceneManager::Exists("testscene")) TRACE("EXISTS");


            

            float accumulator = 0;
            int countfps = 0;
            float r = 0.0f;
            float increment = 0.05f; 
            while (!glfwWindowShouldClose(window))
            {
                DeltaTime::Update();
                accumulator += DeltaTime::Get();
                if (DeltaTime::Get() == 0)
                {
                    TRACE("DT=NULL");
                }
                if (accumulator > 1)
                {
                    TRACE(countfps);
                    countfps = 0;
                    accumulator -= 1;
                }
                else
                {
                    countfps++;
                }
 
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                //renderer.Draw(va, ib, shaderprogram);
                SceneManager::UpdateCurrentScene(window);
                //system.TestRendering(entity1, em, window);
                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            Destruct();
        }

        static void Destruct()
        {
            FileSys::Save();
            glfwTerminate();
            
        }

        

    };

    
}
