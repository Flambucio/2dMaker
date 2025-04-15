#pragma once
#include "../common/Core.h"
#include "../common/GeneralStructs.h"
#include "../Graphics/TextureLoader.h"
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
	
	struct RigidBody :  public Component
	{
		float mass;
		float bounciness;
		RigidBody(float mass, float bounciness)
		{
			this->mass = mass;
			this->bounciness = bounciness;
		}

	};

	struct Collider : public Component
	{
		Rectangle hitbox;
		Collider() : hitbox(0,0,0,0){}
	};

	struct Velocity : public Component
	{
		float dx;
		float dy;
		Velocity(float dx,float dy)
		{
			this->dx = dx;
			this->dy = dy;
		}
	};

	
	struct TextureComponent : public Component
	{
		int orderInLayer;
		std::string name;
		bool exists;
		TextureComponent(const std::string& name,int orderInLayer) : orderInLayer(orderInLayer) , name(name)
		{ 
			if (TextureLoader::Exists(name))
			{
				this->exists = true;
				return;
			}
			this->exists = false;
			WARN("Texture name not found!");
		}

	};
}