#pragma once
#include "Entity.h"
#include "Systems/RenderingSystem.h"
#include "Systems/Physics/Physics.h"`
#include "Systems/Scripting/ScriptSystem.h"
#include "Systems/Physics/RigidBody.h"
namespace D2Maker
{
	class SystemManager
	{
	private:
		std::vector<std::unique_ptr<System>> systems;
	public:
		SystemManager(GLFWwindow* window)
		{
			systems.push_back(std::make_unique<RigidBodySystem>());
			systems.push_back(std::make_unique<ScriptSystem>());
			systems.push_back(std::make_unique<Physics>());
			systems.push_back(std::make_unique<RenderSystem>(window));
		}
		void UpdateSystems(EntityManager& em)
		{
			for (const auto&system : systems)
			{
				system->Update(em);
			}
		}
	};
}