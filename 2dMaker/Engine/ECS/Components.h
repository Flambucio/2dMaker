#pragma once
#include "../common/Core.h"
#include "../common/GeneralStructs.h"
#include "../Graphics/TextureLoader.h"
#include "Systems/Scripting/Parser.h"
#include "../Audio/AudioLoader.h"
#include "Systems/Scripting/StatementAbstract.h"
namespace D2Maker
{
	//class Statement;

	struct Component
	{
	public:
		virtual ~Component() = default;
	};

	struct Transform : public Component
	{
	public:
		float x;
		float y;
		float width;
		float height;
		float rotationDegrees;
		float defaultX;
		float defaultY;
		inline Transform(float x, float y, float width, float height, float rotationDegrees)
		{
			this->x				  = x;
			this->y				  = y;
			this->width			  = width;
			this->height		  = height;
			this->rotationDegrees = rotationDegrees;
			this->defaultX		  = x;
			this->defaultY		  = y;
		}
		inline void ResetPos()
		{
			TRACE("resetting pos...");
			x = defaultX;
			y = defaultY;
		}

	};
	
	struct RigidBody :  public Component
	{
	public:
		float mass;
		float bounciness;
		float maxDy;
		float dyAccumulator;
		inline RigidBody(float mass, float bounciness,float maxDy)
		{
			this->mass			 = mass;
			this->bounciness	 = bounciness;
			this->maxDy			 = maxDy;
			this->dyAccumulator  = 0;
		}

	};

	struct Collider : public Component
	{
	public:
		Collider() = default;
	};

	struct Velocity : public Component
	{
	public:
		float dx;
		float dy;
		float dtheta;
		float defaultDx;
		float defaultDy;
		float defaultDtheta;
		inline Velocity(float dx,float dy,float dtheta)
		{
			this->defaultDx		= dx;
			this->defaultDy		= dy;
			this->defaultDtheta = dtheta;
			this->dx		    = this->defaultDx;
			this->dy		    = this->defaultDy;
			this->dtheta		= this->defaultDtheta;
		}
	};

	
	struct TextureComponent : public Component
	{
	public:
		int orderInLayer;
		std::string name;
		bool exists;
		inline TextureComponent(const std::string& name,int orderInLayer) : orderInLayer(orderInLayer) , name(name)
		{ 
			if (TextureLoader::Exists(this->name))
			{
				this->exists = true;
				return;
			}
			this->exists = false;
			WARN("Texture name not found!");
		}

	};

	struct Script : public Component
	{
	public:
		std::string filepath;
		std::vector<std::vector<std::string>> parsedStr;
		std::vector <std::unique_ptr<Statement>> statements;
		inline Script(std::string filepath)
		{
			this->filepath = filepath;
			Parser::ParseString(filepath, parsedStr);
			PRINT_2D_ARRAY_STR(parsedStr);
			
		}
	};

	struct Name : public Component
	{
	public:
		std::string name;
		inline Name(std::string name)
		{
			this->name = name;
		}
	};

	struct AudioComponent : public Component
	{
	public:
		std::string name;
		bool exists;
		inline AudioComponent(const std::string& name)
		{
			this->name = name;
			if (AudioLoader::Exists(this->name))
			{
				this->exists = true;
				return;
			}
			this->exists = false;
			
		}
	};

	struct Timer : public Component
	{
	public:
		float accumulator = 0;
		inline void ResetAcc()
		{
			accumulator = 0;
		}
	};

	struct Camera : public Component
	{
	public:
		bool enableX;
		bool enableY;
		float x;
		float y;
		inline Camera(bool enableX, bool enableY,float x,float y)
		{
			this->enableX = enableX;
			this->enableY = enableY;
			enableX ? this->x = x : this->x = 0;
			enableY ? this->y = y : this->y = 0;//enable x and y only if the boolean is statisfie
			
		}
	};

	struct Animation : public Component
	{
		std::vector<std::string> texNames;
		float timing;
		float accumulator=0;
		float index=0;
		int orderInLayer;
		int currentTextureIndex = -1;
		inline Animation(std::vector<std::string> names,float timing,int orderInLayer)
		{
			this->texNames	   = names;
			this->timing	   = timing;
			this->orderInLayer = orderInLayer;
		}
		inline void ResetIndexTimes()
		{
			index = 0;
			accumulator = 0;
			currentTextureIndex = -1;
		}
	};

	struct Follow : public Component
	{
		float velocity;
		std::string entityToFollow;
		inline Follow(float velocity,std::string entityToFollow)
		{
			this->velocity=velocity;
			this->entityToFollow = entityToFollow;
		}
	};
}