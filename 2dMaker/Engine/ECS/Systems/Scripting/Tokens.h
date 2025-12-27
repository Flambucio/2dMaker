#pragma once
#include "../../../Events/Keys.h"
namespace D2Maker
{


	class Tokens
	{
	public:

        static const std::unordered_map<std::string, D2Maker::Keys> keyMap;
		inline static Keys InterpretKey(std::string key)
		{
            

            auto it = keyMap.find(key);
            if (it != keyMap.end())
                return it->second;

            return D2Maker::Keys::NUL;

		}

	};

}
