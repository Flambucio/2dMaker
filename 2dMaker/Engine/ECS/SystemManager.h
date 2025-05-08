#pragma once
#include "Entity.h"
#include "Systems/Render/RenderingSystem.h"
#include "Systems/Physics/Physics.h"`
#include "Systems/Scripting/ScriptSystem.h"
#include "Systems/Physics/RigidBody.h"
#include "Systems/Render/CameraSystem.h"
#include "Systems/Render/Animation.h"
#include "Systems/AudioSystem.h"
#include "Systems/Physics/FollowSystem.h"
namespace D2Maker
{
	class SystemManager
	{
	private:
		std::vector<std::unique_ptr<System>> systems;
		AudioSystem as;
	public:
		SystemManager(GLFWwindow* window)
		{
			systems.push_back(std::make_unique<RigidBodySystem>());
			systems.push_back(std::make_unique<FollowSystem>());
			systems.push_back(std::make_unique<ScriptSystem>());
			systems.push_back(std::make_unique<Physics>());
			systems.push_back(std::make_unique<CameraSystem>());
			systems.push_back( std::make_unique<AnimationSystem>());
			systems.push_back(std::make_unique<RenderSystem>(window));
		}
		void UpdateSystems(EntityManager& em)
		{
			for (const auto&system : systems)
			{
				system->Update(em);
			}
		}
		AudioSystem& GetAudioSystem()
		{
			return as;
		}
	};
}