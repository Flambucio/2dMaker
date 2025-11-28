#include "Window.h"
using namespace D2Maker;
//TODO:
//Implement Correct Config
//Variable management
//ComponentTraits
//Batch Rendering
//Physics Engine Fix
//Animation Controller
void Window::Initialize()
{
    // 1. GLFW INIT
    if (!glfwInit()) {
        ERROR("failed to init glfw");
        return;
    }

    FileSys::LoadConfigs();
    /*if (GameOptions::logoFile != "")
    {
        std::string logoPath = "Projects/" + FileSys::currentProject + "/Resources/Textures/" + GameOptions::logoFile + ".png";
        if (FileExists(logoPath))
        {
            glfwSetWindowIcon(window, 1, &FileSys::LoadImage(logoPath));
        }
    }*/
    TRACE("GAMENAMEGAMENAMEGAMENAMEGAMENAMEGAMENAMEGAMENAME");
    TRACE(GameOptions::gameName);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // 2. CREA FINESTRA PRINCIPALE (GAME)
    window = glfwCreateWindow(GameOptions::defaultWidth, GameOptions::defaultHeight, GameOptions::gameName.c_str(), NULL, NULL);
    if (!window) {
        ERROR("failed to create window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowAspectRatio(window, 16, 9);
    glfwSwapInterval(1);

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
    mainImGuiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(mainImGuiContext);
    glfwMakeContextCurrent(guiWindow);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(guiWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
    if (!context || alcMakeContextCurrent(context) == ALC_FALSE)
    {
        ERROR("failed to create or set OpenAL context");
        return;
    }

    TRACE("USING DEVICE=");
    TRACE(alcGetString(device, ALC_DEVICE_SPECIFIER));




}
void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_F11)
        {
            ToggleFullscreen();
        }
        EventManager::PushEvent(static_cast<Keys>(key), EventType::HOLD);

        if (!EventManager::GetConsumedKeys()[static_cast<Keys>(key)])
        {
            EventManager::PushEvent(static_cast<Keys>(key), CLICK);
            EventManager::GetConsumedKeys()[static_cast<Keys>(key)] = true;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        EventManager::GetConsumedKeys()[static_cast<Keys>(key)] = false;
        EventManager::ReleaseKey(static_cast<Keys>(key));
    }
}

void Window::ToggleFullscreen()
{
    fullscreen = !fullscreen;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    if (fullscreen)
    {
        glfwGetWindowPos(window, &windowedX, &windowedY);

        // Passa a fullscreen
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        int width, height;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glFinish();
    }
    else
    {
        glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, GameOptions::defaultWidth,
            GameOptions::defaultHeight, 0);
    }
}
GLFWwindow* Window::ConstructPreviewWindow()
{
    GLFWwindow* pWindow = nullptr;
    pWindow = glfwCreateWindow(500, 500, "Texture preview", nullptr, window);
    if (!pWindow)
    {
        return nullptr;
    }
    return pWindow;
}

void Window::RunWindow()
{
    FileSys::LoadAssets();
    FileSys::LoadProjectJsons();
    std::string traceAssStr = "";
    for (const auto& element : TextureLoader::GetMap())
    {
        traceAssStr += element.first + "|" + element.second->filepath + "\n";
    }
    for (const auto& element : AudioLoader::GetMap())
    {
        traceAssStr += element.first + "|" + element.second->GetFilePath() + "\n";
    }
    TRACE(traceAssStr)
       
    FileSys::LoadScenes();
    editor.LoadStartingScenesNames();
    TRACE("defaultscene" + SceneManager::defaultScene);
    SceneManager::SelectScene(SceneManager::defaultScene);
    for (auto& scene : SceneManager::scenes)
    {
        ASTBuilder::ParseScriptSystems(scene.second->em);
    }
    if (GameOptions::fullScreen)
    {
        ToggleFullscreen();
    }
    float accumulator = 0;
    int countfps = 0;

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
        SceneManager::UpdateCurrentScene(window, editor.runGameFlag);
        glfwMakeContextCurrent(guiWindow);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        ImGui::SetCurrentContext(mainImGuiContext);
        editor.Update();
        editor.GetPreviewWindowRef().Update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Destruct();
}

void Window::Destruct()
{
    if (editor.GetPreviewWindowRef().windowOpen)
    {
        editor.GetPreviewWindowRef().Destruct();
    }
    glfwDestroyWindow(window);
    glfwDestroyWindow(guiWindow);
    FileSys::Save();
    glfwTerminate();

}