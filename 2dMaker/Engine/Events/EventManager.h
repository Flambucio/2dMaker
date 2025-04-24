#pragma once
#include "Keys.h"
#include "../common/Core.h"
namespace D2Maker
{
	static class EventManager
	{
	private:
		static std::unordered_set<Keys> pressedKeys;
	public:
		static void PushEvent(const Keys& e)
		{
			pressedKeys.insert(e);
		}

		static void ReleaseKey(Keys key)
		{
			pressedKeys.erase(key);
		}

		static bool IsKeyPressed(Keys key)
		{
			return pressedKeys.find(key) != pressedKeys.end();
		}


	};
}
