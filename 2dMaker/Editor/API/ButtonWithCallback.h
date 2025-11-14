#pragma once
#include "Button.h"
#define CALLBACKFUNCTION std::function<void(Args...)>
namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			//template<typename... Args>
			//using CallbackFunction = std::function<void(Args...)>;
			template<typename... Args>
			class ButtonWithCallback
			{
				CALLBACKFUNCTION callBack;
				Button button;
			public:
				inline ButtonWithCallback(int width,int height,std::string id,
					CALLBACKFUNCTION callBack) : button(width,height,id)
				{
					this->callBack = callBack;
				}

				inline void Update(Args&&... args)
				{
					if (button.Active())
					{
						if (callBack)
						{
							callBack(args...);
						}
					}
					
				}
			};


		}
	}
}
#undef CALLBACKFUNCTION