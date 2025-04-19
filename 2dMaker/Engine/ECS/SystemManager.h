#pragma once
#include "Entity.h"
#include "Systems/RenderingSystem.h"
#include "Systems/Physics.h"
namespace D2Maker
{
	class SystemManager
	{
	private:
		std::vector<std::unique_ptr<System>> systems;
	public:
		SystemManager(GLFWwindow* window)
		{
			systems.push_back(std::make_unique<Physics>());
			systems.push_back(std::make_unique<RenderSystem>(window));
			
		}
		template<typename T>
		void RegisterSystem()
		{
			systems.push_back(std::make_shared<T>);

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