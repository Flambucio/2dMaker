#include "Application/Window.h"
#include "Graphics/TextureLoader.h"
#include "Events/EventManager.h"
#include "ECS/Systems/Scripting/Tokens.h"

namespace D2Maker
{
	GLFWwindow* Window::window = nullptr;
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
    {"Q", D2Maker::Keys::Q},
    {"W", D2Maker::Keys::W},
    {"E", D2Maker::Keys::E},
    {"R", D2Maker::Keys::R},
    {"T", D2Maker::Keys::T},
    {"Y", D2Maker::Keys::Y},
    {"U", D2Maker::Keys::U},
    {"I", D2Maker::Keys::I},
    {"O", D2Maker::Keys::O},
    {"P", D2Maker::Keys::P},
    {"A", D2Maker::Keys::A},
    {"S", D2Maker::Keys::S},
    {"D", D2Maker::Keys::D},
    {"F", D2Maker::Keys::F},
    {"G", D2Maker::Keys::G},
    {"H", D2Maker::Keys::H},
    {"J", D2Maker::Keys::J},
    {"K", D2Maker::Keys::K},
    {"L", D2Maker::Keys::L},
    {"Z", D2Maker::Keys::Z},
    {"X", D2Maker::Keys::X},
    {"C", D2Maker::Keys::C},
    {"V", D2Maker::Keys::V},
    {"B", D2Maker::Keys::B},
    {"N", D2Maker::Keys::N},
    {"M", D2Maker::Keys::M},
    {"LEFT_MOUSE", D2Maker::Keys::LEFT},
    {"RIGHT_MOUSE", D2Maker::Keys::RIGHT},
    };
}



