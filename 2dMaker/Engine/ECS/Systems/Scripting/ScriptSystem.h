#pragma once
#include "Interpreter.h"
#include "Parser.h"

namespace D2Maker
{
	class ScriptSystem : public System
	{
		void Update(EntityManager& em)
		{
			for (Entity entity : em.aliveEntities)
			{
				if (!em.hasComponent<Script>(entity))
				{
					continue;
				}
				
					


			}
		}
	};

}

