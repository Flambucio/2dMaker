#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <fstream>
#include <sstream>
#include <exception>
#include <map>
#include <queue>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <filesystem>


#include "Log.h"
#include "Deltatime.h"


#include "../../Lib/stbimage/stb_image.h"
#include "../../Lib/stbvorbis/stb_vorbis.h"

#define GLEW_STATIC
#include "../../Lib/GLEW/include/GL/glew.h"
#include "../../Lib/glfw/include/GLFW/glfw3.h"

#include "../../Lib/openAL/include/AL/al.h"
#include "../../Lib/openAL/include/AL/alc.h"

#define CAPTION "game"
#define DEFAULT_WIN_H  720
#define DEFAULT_WIN_W 1280

#define VIRTUAL_WIDTH 1600
#define VIRTUAL_HEIGHT  900

#define ASPECT_RATIO = 16.0/9.0
#define INVERSE_ASPECT_RATIO = 9.0/16.0
#define SHADER_PATH "Engine/Resources/Shaders/Shader.shader"
