#pragma once
#include "../ECS/SystemManager.h"
namespace D2Maker
{	
	template<typename Texture>//in futuro rimpiazzato dal componente texture direttamente
	struct OrderInLayerComp
	{
		bool operator()(std::unique_ptr<Texture>& a, std::unique_ptr<Texture>& b)
		{
			return a->orderInLayer < b->orderInLayer;
		}
	};

	template<typename Texture>
	class TextureRenderer
	{
	private:
		std::priority_queue<Texture, std::vector<Texture>, OrderInLayerComp<Texture>> queue;

	public:

		void Render()
		{
			while (!queue.empty())
			{
				auto t = queue.top();
				//metodo per rendering
				queue.pop();
			}
		}
	};
}