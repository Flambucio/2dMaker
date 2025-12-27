#pragma once
#include "Keys.h"
#include "../common/Core.h"
namespace D2Maker
{

	enum EventType 
	{
		HOLD,
		CLICK
	};

	static class EventManager
	{
	private:
		static std::unordered_set<Keys> pressedKeys;
		static std::unordered_map<Keys,bool> consumedKeys;
		static std::unordered_set<Keys> justClicked;
	public:

		inline static std::unordered_map<Keys, bool>& GetConsumedKeys()
		{
			return consumedKeys;
		}

		inline static void PushEvent(const Keys& key,const EventType e)
		{
			if (e == HOLD)
			{
				pressedKeys.insert(key);
			}
			else 
			{
				justClicked.insert(key);
			}
			
		}

		inline static void ReleaseKey(Keys key)
		{
			pressedKeys.erase(key);
		}

		inline static bool IsKeyPressed(Keys key)
		{
			return pressedKeys.find(key) != pressedKeys.end();
		}

		inline static bool IsKeyPressedOnce(Keys key)
		{
			return justClicked.find(key) != justClicked.end();
		}

		inline static void ClearJustClickedKeys() 
		{
			justClicked.clear();
		}
	};
}
