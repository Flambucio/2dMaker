#pragma once
#include "../../common/Core.h"
class VertexBuffer
{
private:
	unsigned int rendererID;
public:
	VertexBuffer(const void* data, unsigned int size)
	{
		glGenBuffers(1, &rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	~VertexBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	void Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	}
	void Unbind() const 
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};
