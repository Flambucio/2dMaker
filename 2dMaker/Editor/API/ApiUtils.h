#pragma once
#ifndef APIUTILS
#define APIUTILS
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
				inline static int GetValidIndex(std::vector<T> values, int index)
				{
					if (values.empty()) return -1;
					if (index < 0 || index >= static_cast<int>(values.size())) return 0;
					return index;
				}
			};
		}
	}
}
#endif
