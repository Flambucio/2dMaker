#pragma once

#include "../../Graphics/Texture.h"
#include "../../Graphics/Renderer.h"
#include "Sysbase.h"

namespace D2Maker
{
	struct OrderInLayerComp
	{
		bool operator()(const std::pair<Entity, int>& a, const std::pair<Entity, int>& b)
		{
			return a.second < b.second;
		}
	};

    class RenderSystem : public System
    {
    private:
        GLFWwindow* window;
        Shaders shaderProgram;
        //IndexBuffer ibo;
        Renderer renderer;
        VertexBufferLayout layout;
        unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

        std::priority_queue<
            std::pair<Entity, int>,
            std::vector<std::pair<Entity, int>>,
            OrderInLayerComp> queue;

    public:
        RenderSystem(GLFWwindow*window) //:ibo(indices, 6)       
        {
            this->window=window;
            layout.Push<float>(2);
            layout.Push<float>(2);
            shaderProgram.Bind();
        }

        void Update(EntityManager& em)
        {
            PushToQueue(em);
            RenderQueue(em);
        }

    private:
        void PushToQueue(EntityManager& em)
        {
            for (auto entity : em.aliveEntities)
            {
                TextureComponent* texcomponent = em.getComponent<TextureComponent>(entity);
                if (!texcomponent || !texcomponent->exists) continue;

                queue.push({ entity, texcomponent->orderInLayer });

                //TRACE("Pushed entity with texture: " +texcomponent->name);
            }
        }

        std::array<float, 4> ConvertToNdc(Transform* transform)
        {
            return {
                ((2 * transform->x) / VIRTUAL_WIDTH) - 1,
                1 - ((2 * transform->y) / VIRTUAL_HEIGHT),
                ((2 * transform->width) / VIRTUAL_WIDTH),
                -((2 * transform->height) / VIRTUAL_HEIGHT)
            };
        }

        std::array<float, 16> GetVertices(Transform* transform)
        {
            std::array<float, 4> v = ConvertToNdc(transform);
            return {
                v[0],         v[1],         0.0f, 0.0f,
                v[0] + v[2],  v[1],         1.0f, 0.0f,
                v[0] + v[2],  v[1] + v[3],  1.0f, 1.0f,
                v[0],         v[1] + v[3],  0.0f, 1.0f
            };
        }

        void RenderQueue(EntityManager& em)
        {
            //renderer.Clear();
            //shaderProgram.Bind();
            
            renderer.Clear();
            shaderProgram.Bind();
            while (!queue.empty())
            {
                Entity entity = queue.top().first;
                queue.pop();
                unsigned int indices[] = { 0,1,2,2,3,0 };
           

                IndexBuffer ibo(indices, 6);
                //Renderer renderer;
                //Entity entity = 0;
                TextureComponent* texcomponent = em.getComponent<TextureComponent>(entity);
                Transform* transformcomponent = em.getComponent<Transform>(entity);
                std::array<float, 16> vert = GetVertices(transformcomponent);
                VertexArray vao;
                vao.Bind();
                VertexBuffer vbo(vert.data(), 4 * 4 * sizeof(float));
                vao.AddBuffer(vbo, layout);

                TextureLoader::BindTexture(texcomponent->name);
                renderer.Draw(vao, ibo, shaderProgram);
                
            }
        }
    };

}
