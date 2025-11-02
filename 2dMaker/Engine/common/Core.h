#pragma once
//Default libs
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
#include <cmath>
#include <variant>

//Core Files
#include "Log.h"
#include "Deltatime.h"

//ImGui
#include "../../Lib/imgui/imgui.h"
#include "../../Lib/imgui/imconfig.h"
#include "../../Lib/imgui/backends/imgui_impl_glfw.h"
#include "../../Lib/imgui/backends/imgui_impl_opengl3.h"
//STB_IMAGE
#include "../../Lib/stbimage/stb_image.h"
#include "../../Lib/stbvorbis/stb_vorbis.h"
//GLEW
#define GLEW_STATIC
#include "../../Lib/GLEW/include/GL/glew.h"
//GLFW
#include "../../Lib/glfw/include/GLFW/glfw3.h"

//OpenAL
#include "../../Lib/openAL/include/AL/al.h"
#include "../../Lib/openAL/include/AL/alc.h"

//JSON PARSER
#include "../../Lib/Json/json.hpp"





//Window default (GUI)
#define DEFAULT_WIN_H  720
#define DEFAULT_WIN_W 1280

//Virtual coords
//#define VIRTUAL_WIDTH 1600
//#define VIRTUAL_HEIGHT  900

#define CENTER_X 800
#define CENTER_Y 450

//#define ASPECT_RATIO = 16.0/9.0
#define INVERSE_ASPECT_RATIO = 9.0/16.0
#define SHADER_PATH "Engine/Resources/Shaders/Shader.shader"

//this macro is for getting the current scene, it was not created immediately during development so
//you may find places without this one
#define CURRENT_SCENE SceneManager::GetScene(SceneManager::currentScene)

namespace D2Maker
{
    template<typename T>
    inline void SetToVector(const std::unordered_set<T>& set, std::vector<T>& vec) noexcept
    {
        vec.clear();
        for (auto element : set)
        {
            vec.push_back(element);
        }
    }
    template<typename T,typename U>
    inline void FirstElementOfMapToVector(const std::unordered_map<T, U>& map, std::vector<T>& vec) noexcept
    {
        for (auto& element : map)
        {
            vec.push_back(element.first);
        }
    }

    inline void SliceStringReference(std::string& str,const int &startIndex,const int &lastIndex) noexcept
    {
        std::string buffer="";
        for (int i = 0;i < lastIndex+1;i++)
        {
            if (i >= startIndex) buffer += str[i];
        }

        str = buffer;
    }

    inline std::string SliceStringValue(std::string str, const int& startIndex, const int& lastIndex) noexcept
    {
        std::string buffer = "";
        for (int i = 0;i < lastIndex + 1;i++)
        {
            if (i >= startIndex) buffer += str[i];
        }

        return buffer;
    }

    template<typename NUM>
    inline bool ConvertStringToNum(const std::string &str, NUM& value) noexcept
    {
        return false;
    }

    template<>
    inline bool ConvertStringToNum<float>(const std::string &str, float& value) noexcept
    {
        std::istringstream iss(str);
        iss >> std::noskipws >> value;
        if (!(iss.eof() && !iss.fail())) return false;
        return true;
    }

    template<>
    inline bool ConvertStringToNum<double>(const std::string &str, double& value) noexcept
    {
        std::istringstream iss(str);
        iss >> std::noskipws >> value;
        if (!(iss.eof() && !iss.fail())) return false;
        return true;
    }

    template<>
    inline bool ConvertStringToNum<int>(const std::string& str, int& value) noexcept
    {
        std::istringstream iss(str);
        iss >> std::noskipws >> value;
        if (!(iss.eof() && !iss.fail())) return false;
        return true;
    }


    namespace GameOptions
    {
        inline int virtualWidth=1600;
        inline int virtualHeight=900;
        inline int defaultHeight=720;
        inline int defaultWidth=1280;
        inline int aspectRatioNumerator = 16;
        inline int aspectRatioDenominator = 9;
        inline std::string aspectRatioStr = "16/9";
        inline std::string gameName="Game";
        inline std::string logoFile="";
        inline std::string defaultScene="";
        inline bool fullScreen=false;


    }
    
    
}
