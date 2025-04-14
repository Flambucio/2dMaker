#pragma once
#include "GL/VertexArray.h"
#include "GL/IndexBuffer.h"
#include "GL/Shaders.h"
namespace D2Maker
{
	class Renderer
	{
	public:
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shaders& shader) const
		{
			shader.Bind();
			va.Bind();
			ib.Bind();
			glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

		}

		void Clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
	};
}

