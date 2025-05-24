#pragma once
#include "Button.h"
namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			template<typename... Args>
			class ButtonWithCallback
			{
				std::function<void(Args...)> callBack;
				Button button;
			public:
				ButtonWithCallback(int width,int height,std::string id,
					std::function<void(Args...)> callBack) : button(width,height,id)
				{
					this->callBack = callBack;
				}

				void Update(Args&&... args)
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