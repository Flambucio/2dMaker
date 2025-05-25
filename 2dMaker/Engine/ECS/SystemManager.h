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
		AudioSystem as;
		RigidBodySystem rbs;
		FollowSystem fs;
		ScriptSystem ss;
		Physics ps;
		CameraSystem cs;
		AnimationSystem ans;
		RenderSystem rs;
	public:
		SystemManager(GLFWwindow* window) : rs(window)
		{

		}
		void UpdateSystems(EntityManager& em,bool runGameFlag)
		{
			if (runGameFlag)
			{
				rbs.Update(em);
				fs.Update(em);
				ss.Update(em);
				ps.Update(em);
				cs.Update(em);
				ans.Update(em);
			}
			rs.Update(em);


		}
		AudioSystem& GetAudioSystem()
		{
			return as;
		}
	};
}