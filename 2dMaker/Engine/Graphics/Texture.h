#pragma once
#include "Renderer.h"
namespace D2Maker
{


	class Texture
	{
	private:
	public:
		unsigned int rendererID;
		std::string filepath;
		unsigned char* localBuffer;
		int width;
		int height;
		int bpp;
	public:
		inline Texture(const std::string& filepath) : rendererID(0), filepath(filepath), localBuffer(nullptr), width(0), height(0),
			bpp(0)
		{

			stbi_set_flip_vertically_on_load(false);
			localBuffer = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);

			glGenTextures(1, &rendererID);
			glBindTexture(GL_TEXTURE_2D, rendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
			glBindTexture(GL_TEXTURE_2D, 0);

			if (localBuffer)
			{
				stbi_image_free(localBuffer);
			}
		}
		inline ~Texture()
		{
			glDeleteTextures(1, &rendererID);
		}

		inline void Bind(unsigned int slot = 0) const
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, rendererID);
		}
		inline void Unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		inline int GetWidth() const
		{
			return width;
		}

		inline int GetHeight() const
		{
			return height;
		}

		inline std::string GetPath() const
		{
			return filepath;
		}
	};

}