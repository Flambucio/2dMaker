#pragma once
#include "../../common/Core.h"
namespace D2Maker
{


	class VertexBuffer
	{
	private:
		unsigned int rendererID;
	public:
		inline VertexBuffer(const void* data, unsigned int size)
		{
			glGenBuffers(1, &rendererID);
			glBindBuffer(GL_ARRAY_BUFFER, rendererID);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		}
		inline ~VertexBuffer()
		{
			glDeleteBuffers(1, &rendererID);
		}

		inline void Bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		}
		inline void Unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	};

}