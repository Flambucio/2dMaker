#include "Application/Window.h"
#include "Graphics/TextureLoader.h"

namespace D2Maker
{
	GLFWwindow* Window::window = nullptr;
	std::unordered_map<std::string, std::unique_ptr<Texture>> TextureLoader::textures;
}



