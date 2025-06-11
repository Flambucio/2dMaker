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
        //PreviewTextureWindow previewTextureWindow;
    public:


        static void Initialize()
        {
            // 1. GLFW INIT
            if (!glfwInit()) {
                ERROR("failed to init glfw");
                return;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

            // 2. CREA FINESTRA PRINCIPALE (GAME)
            window = glfwCreateWindow(DEFAULT_WIN_W, DEFAULT_WIN_H, CAPTION, NULL, NULL);
            if (!window) {
                ERROR("failed to create window");
                glfwTerminate();
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSetWindowAspectRatio(window, 16, 9);

            // 3. GLEW SOLO UNA VOLTA
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if (err != GLEW_OK) {
                ERROR("failed to init glew");
                ERROR((const char*)glewGetErrorString(err));
                glfwTerminate();
                return;
            }

            // 4. CREA FINESTRA GUI (condivisione contesto)
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            guiWindow = glfwCreateWindow(1280, 720, "Editor", NULL, window); // Condividi contesto OpenGL!
            if (!guiWindow) {
                ERROR("failed to create IMGUI window");
                glfwTerminate();
                return;
            }
            ImGuiContext* mainImGuiContext = ImGui::CreateContext();
            ImGui::SetCurrentContext(mainImGuiContext);
            glfwMakeContextCurrent(guiWindow);
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(guiWindow, true);
            ImGui_ImplOpenGL3_Init("#version 330");

            // 6. SETUP GLOBALE
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwSetKeyCallback(window, KeyCallback);
            glfwSetFramebufferSizeCallback(guiWindow, framebuffer_size_callback);
            glfwSetKeyCallback(guiWindow, KeyCallback);


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
            TRACE(alcGetString(device, ALC_DEVICE_SPECIFIER));
            



        }
        static inline void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
            if (action == GLFW_PRESS)
            {
                EventManager::PushEvent(static_cast<Keys>(key));
            }
            else if (action == GLFW_RELEASE)
            {
                EventManager::ReleaseKey(static_cast<Keys>(key));
            }
        }

        static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            glfwMakeContextCurrent(window);
            glViewport(0, 0, width, height);
        }
        static bool FileExists(const std::string& path)
        {
            std::ifstream file(path);
            return file.good();
        }


        static GLFWwindow* ConstructPreviewWindow()
        {
            GLFWwindow* pWindow = nullptr;
            pWindow = glfwCreateWindow(500, 500, "Texture preview", nullptr, window);
            if (!pWindow)
            {
                return nullptr;
            }
            return pWindow;
        }

        static void RunWindow()
        {
            FileSys::LoadAssets();
            std::string traceAssStr = "";
            for (const auto &element : TextureLoader::GetMap())
            {
                traceAssStr += element.first + "|" + element.second->filepath + "\n";
            }
            for (const auto& element : AudioLoader::GetMap())
            {
                traceAssStr += element.first + "|" + element.second->GetFilePath() +"\n";
            }
            TRACE(traceAssStr)
            FileSys::LoadScenes();
            editor.LoadStartingScenesNames();
            TRACE("defaultscene" +  SceneManager::defaultScene);
            SceneManager::SelectScene(SceneManager::defaultScene);

          
            float accumulator = 0;
            int countfps = 0;
            float r = 0.0f;
            float increment = 0.05f; 
            while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(guiWindow))
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
                glfwMakeContextCurrent(window);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                SceneManager::UpdateCurrentScene(window,editor.runGameFlag);
                editor.Update();
                editor.GetPreviewWindowRef().Update();
                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            Destruct();
        }

        static void Destruct()
        {
            glfwDestroyWindow(window);
            glfwDestroyWindow(guiWindow);
            FileSys::Save();
            glfwTerminate();
            
        }

        

    };


    

    
}
