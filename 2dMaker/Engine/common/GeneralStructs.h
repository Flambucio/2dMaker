#pragma once
#include "Core.h"

namespace D2Maker
{
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		Color(uint8_t r,uint8_t g,uint8_t b,uint8_t a=255)
		{
			this->a = a;
			this->b = b;
			this->g = g;
			this->r = r;
		}
	};

	struct Rectangle
	{
		float width;
		float height;
		float x;
		float y;
		Rectangle(float x, float y, float width, float height)
		{
			this->height = height;
			this->width = width;
			this->x = x;
			this->y = y;
		}
	};

	struct Vec2
	{
		float x;
		float y;
		Vec2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		Vec2()
		{
			this->x = 0;
			this->y = 0;
		}
	};


}
