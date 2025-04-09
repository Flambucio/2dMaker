#pragma once
#include "../common/Core.h"
#include "Shaders.h"

namespace D2Maker
{
	class TextureOBJ
	{
	public:
		std::string path;
		GLuint textureID;
		GLuint VAO;
		GLuint VBO;
		Shaders shaders;
		GLuint shaderProgram;

		TextureOBJ(std::string path)
		{
			
		}

		void LoadTexture()
		{

		}

	};
}