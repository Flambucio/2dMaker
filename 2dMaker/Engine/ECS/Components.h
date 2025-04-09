#pragma once
#include "../common/Core.h"
#include "../common/GeneralStructs.h"
namespace D2Maker
{


	struct Component
	{
		virtual ~Component() = default;
	};

	struct Transform : public Component
	{
		float x;
		float y;
		float width;
		float height;
		float rotationDegrees;
		Transform(float x, float y, float width, float height, float rotationDegrees)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->rotationDegrees = rotationDegrees;
		}

	};
	
	struct RigidBody
	{
		float mass;
		float bounciness;
		RigidBody(float mass, float bounciness)
		{
			this->mass = mass;
			this->bounciness = bounciness;
		}

	};

	struct Collider
	{
		Rectangle hitbox;
		float &x;
		float &y;
		float &width;
		float &height;
		Collider(float& x, float& y, float& width, float& height) : x(x) , y(y) , width(width),height(height) ,
			hitbox(width, height, x, y)
		{
		}
	};

	struct Velocity
	{
		float dx;
		float dy;
		Velocity(float dx,float dy)
		{
			this->dx = dx;
			this->dy = dy;
		}
	};
}