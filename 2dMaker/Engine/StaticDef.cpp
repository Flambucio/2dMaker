#include "Application/Window.h"
#include "Filesystem/Filesystem.h"
#include "../Editor/API/WindowGUI.h"

namespace D2Maker
{
	GLFWwindow* Window::window = nullptr;
    GLFWwindow* Window::guiWindow = nullptr;
    GUI::Editor Window::editor(Window::guiWindow);
    int Window::m_Width = DEFAULT_WIN_W;
    int Window::m_Height=DEFAULT_WIN_H;
	std::unordered_map<std::string, std::unique_ptr<Texture>> TextureLoader::textures;
	std::unordered_set<Keys> EventManager::pressedKeys;
    const std::unordered_map<std::string, D2Maker::Keys> Tokens::keyMap = {
    {"ZERO", D2Maker::Keys::ZERO},
    {"ONE", D2Maker::Keys::ONE},
    {"TWO", D2Maker::Keys::TWO},
    {"THREE", D2Maker::Keys::THREE},
    {"FOUR", D2Maker::Keys::FOUR},
    {"FIVE", D2Maker::Keys::FIVE},
    {"SIX", D2Maker::Keys::SIX},
    {"SEVEN", D2Maker::Keys::SEVEN},
    {"EIGHT", D2Maker::Keys::EIGHT},
    {"NINE", D2Maker::Keys::NINE},
    {"SPACE", D2Maker::Keys::SPACE},
    {"LEFT_SHIFT", D2Maker::Keys::LEFT_SHIFT},
    {"RIGHT_SHIFT", D2Maker::Keys::RIGHT_SHIFT},
    {"LEFT_CTR", D2Maker::Keys::LEFT_CTR},
    {"RIGHT_CTR", D2Maker::Keys::RIGHT_CTR},
    {"LEFT_ALT", D2Maker::Keys::LEFT_ALT},
    {"RIGHT_ALT", D2Maker::Keys::RIGHT_ALT},
    {"TAB", D2Maker::Keys::TAB},
    {"CAPS", D2Maker::Keys::CAPS},
    {"Q", D2Maker::Keys::Q_K},
    {"W", D2Maker::Keys::W_K},
    {"E", D2Maker::Keys::E_K},
    {"R", D2Maker::Keys::R_K},
    {"T", D2Maker::Keys::T_K},
    {"Y", D2Maker::Keys::Y_K},
    {"U", D2Maker::Keys::U_K},
    {"I", D2Maker::Keys::I_K},
    {"O", D2Maker::Keys::O_K},
    {"P", D2Maker::Keys::P_K},
    {"A", D2Maker::Keys::A_K},
    {"S", D2Maker::Keys::S_K},
    {"D", D2Maker::Keys::D_K},
    {"F", D2Maker::Keys::F_K},
    {"G", D2Maker::Keys::G_K},
    {"H", D2Maker::Keys::H_K},
    {"J", D2Maker::Keys::J_K},
    {"K", D2Maker::Keys::K_K},
    {"L", D2Maker::Keys::L_K},
    {"Z", D2Maker::Keys::Z_K},
    {"X", D2Maker::Keys::X_K},
    {"C", D2Maker::Keys::C_K},
    {"V", D2Maker::Keys::V_K},
    {"B", D2Maker::Keys::B_K},
    {"N", D2Maker::Keys::N_K},
    {"M", D2Maker::Keys::M_K},
    {"LEFT_MOUSE", D2Maker::Keys::LEFT},
    {"RIGHT_MOUSE", D2Maker::Keys::RIGHT},
    };
    std::unordered_map<std::string, std::unique_ptr<Audio>> AudioLoader::audios = {};
    std::function<void(const std::string&)> Interpreter::sceneChangeCallback = [](const std::string& name) {
        SceneManager::SelectScene(name);};
    std::unordered_map<std::string, std::unique_ptr<Scene>> SceneManager::scenes;
    std::string SceneManager::currentScene = "";
    GLFWwindow* SceneManager::window = nullptr;
    std::string FileSys::currentProject = "";
    const std::unordered_set<std::string> EntityManager::componentsTypesStr = {
        "TRANSFORM",
        "COLLIDER",
        "VELOCITY",
        "TIMER",
        "NAME",
        "AUDIO",
        "SCRIPT",
        "TEXTURE",
        "ANIMATION",
        "RIGIDBODY",
        "CAMERA",
        "FOLLOW"

    };
    std::unordered_set<std::string> FileSys::projectNames = {};
    std::string SceneManager::defaultScene = "";
    ImGuiWindowFlags D2Maker::GUI::GUIAPI::GUIWindow::flags = 
        ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoTitleBar;
}



