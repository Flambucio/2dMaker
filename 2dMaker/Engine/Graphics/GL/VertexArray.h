#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
namespace D2Maker
{


	class VertexArray
	{
	private:
		unsigned int rendererID;
	public:
		inline VertexArray()
		{
			glGenVertexArrays(1, &rendererID);
		}
		inline ~VertexArray()
		{
			glDeleteVertexArrays(1, &rendererID);
		}

		inline void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
		{
			Bind();
			vb.Bind();
			const auto& elements = layout.GetElements();
			unsigned int offset = 0;
			for (int i = 0;i < elements.size();i++)
			{
				const auto& element = elements[i];
				glVertexAttribPointer(i, element.count, element.type,
					element.normalized, layout.GetStride(), (const void*)offset);
				offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
				glEnableVertexAttribArray(i);
			}


		}

		inline void Bind() const
		{
			glBindVertexArray(rendererID);
		}

		inline void Unbind() const
		{
			glBindVertexArray(0);
		}

	};

}