#pragma once
#include "Entity.h"
namespace D2Maker
{
	class System
	{
	public:
		virtual void Update()
		{

		}
	};
	class SystemManager
	{
	private:
		std::vector<std::unique_ptr<System>> systems;
	public:
		template<typename T>
		void RegisterSystem()
		{
			systems.push_back(std::make_shared<T>);

		}

		void UpdateSystems()
		{
			for (const auto&system : systems)
			{
				system->Update();
			}
		}
	};
}