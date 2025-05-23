#pragma once
#include "../../Engine/EngineExport.h"

namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class APIUtils
			{
			public:
				template<typename T>
				static int GetValidIndex(std::vector<T> values, int index)
				{
					if (values.empty()) return -1;
					if (index < 0 || index >= static_cast<int>(values.size())) return 0;
					return index;
				}
			};
		}
	}
}
