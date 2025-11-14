#pragma once

#include "../../../Graphics/Texture.h"
#include "../../../Graphics/Renderer.h"
#include "../Physics/ColliderFunctions.h"
#include "../Sysbase.h"

namespace D2Maker
{
    template<typename T>
	struct OrderInLayerComp
	{
        bool operator()(const std::pair<T, int>& a, const std::pair<T, int>& b) const
		{
			return a.second > b.second;
		}
	};

    class RenderSystem : public System
    {
    private:
        GLFWwindow* window;
        Shaders shaderProgram;
        Renderer renderer;
        VertexBufferLayout layout;
        unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

        std::priority_queue<
            std::pair<Entity, int>,
            std::vector<std::pair<Entity, int>>,
            OrderInLayerComp<Entity>> queue;

    public:
        inline RenderSystem(GLFWwindow*window) //:ibo(indices, 6)       
        {
            this->window=window;
            layout.Push<float>(2);
            layout.Push<float>(2);
            shaderProgram.Bind();
        }

        inline void Update(EntityManager& em)
        {
            PushToQueue(em);
            RenderQueue(em);
        }

    private:
        void PushToQueue(EntityManager& em);
        std::array<float, 16> GetVertices(Transform* t);
        void RenderQueue(EntityManager& em);
    };

}
