#pragma once
#include "../../common/Core.h"
namespace D2Maker
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:   return 4;
			case GL_UNSIGNED_BYTE:  return 1;
			}
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride = 0;
	public:
		VertexBufferLayout()
		{

		}

		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			elements.push_back({ GL_FLOAT,count,GL_FALSE });
			stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
			stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
			stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
		}

		unsigned int GetStride() const { return stride; };
		std::vector<VertexBufferElement> GetElements() const { return elements; };

	};
}

