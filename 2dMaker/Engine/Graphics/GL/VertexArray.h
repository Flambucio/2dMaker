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
		VertexArray()
		{
			glGenVertexArrays(1, &rendererID);
		}
		~VertexArray()
		{
			glDeleteVertexArrays(1, &rendererID);
		}

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
		{
			Bind();
			vb.Bind();
			const auto& elements = layout.GetElements();
			unsigned int offset = 0;
			for (int i = 0;i < elements.size();i++)
			{
				const auto& element = elements[i];
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, element.count, element.type,
					element.normalized, layout.GetStride(), (const void*)offset);
				offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			}


		}

		void Bind() const
		{
			glBindVertexArray(rendererID);
		}

		void Unbind() const
		{
			glBindVertexArray(0);
		}

	};

}