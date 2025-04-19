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
			this->x				  = x;
			this->y				  = y;
			this->width			  = width;
			this->height		  = height;
			this->rotationDegrees = rotationDegrees;
		}

	};
	
	struct RigidBody :  public Component
	{
		float mass;
		float bounciness;
		float max_dy;
		float dy_accumulator;
		RigidBody(float mass, float bounciness,float max_dy)
		{
			this->mass			 = mass;
			this->bounciness	 = bounciness;
			this->max_dy	     = max_dy;
			this->dy_accumulator = 0;
		}

	};

	struct Collider : public Component
	{
		Collider() = default;
	};

	struct Velocity : public Component
	{
		float dx;
		float dy;
		float default_dx;
		float default_dy;
		Velocity(float dx,float dy)
		{
			this->default_dx = dx;
			this->default_dy = dy;
			this->dx		 = this->default_dx;
			this->dy		 = this->default_dy;
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