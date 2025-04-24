#pragma once

#include "../../Graphics/Texture.h"
#include "../../Graphics/Renderer.h"
#include "Sysbase.h"

namespace D2Maker
{
    template<typename T>
	struct OrderInLayerComp
	{
        bool operator()(const std::pair<T, int>& a, const std::pair<T, int>& b) const
		{
			return a.second < b.second;
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
                if (!texcomponent || !texcomponent->exists) { continue; }

                queue.push({ entity, texcomponent->orderInLayer });

                //TRACE("Pushed entity with texture: " +texcomponent->name);
            }
        }

        std::array<float, 16> GetVertices(Transform* t)
        {
            float angle = t->rotationDegrees * (3.14159265f / 180.0f);
            float s = sin(angle);
            float c = cos(angle);

            float cx = t->x + t->width / 2.0f;
            float cy = t->y + t->height / 2.0f;

            std::array<std::pair<float, float>, 4> corners = {
                std::make_pair(t->x,           t->y),
                std::make_pair(t->x + t->width, t->y),
                std::make_pair(t->x + t->width, t->y + t->height),
                std::make_pair(t->x,           t->y + t->height)
            };

            std::array<float, 16> result;

            for (int i = 0; i < 4; i++)
            {
                float x = corners[i].first;
                float y = corners[i].second;
                float dx = x - cx;
                float dy = y - cy;
                float xRot = dx * c - dy * s + cx;
                float yRot = dx * s + dy * c + cy;
                float ndcX = ((2 * xRot) / VIRTUAL_WIDTH) - 1;
                float ndcY = 1 - ((2 * yRot) / VIRTUAL_HEIGHT);

                
                result[i * 4 + 0] = ndcX;
                result[i * 4 + 1] = ndcY;
                result[i * 4 + 2] = (i == 1 || i == 2) ? 1.0f : 0.0f; 
                result[i * 4 + 3] = (i >= 2) ? 1.0f : 0.0f;           

            }

            return result;
        }

        void RenderQueue(EntityManager& em)
        {
            renderer.Clear();
            shaderProgram.Bind();
            while (!queue.empty())
            {
                Entity entity = queue.top().first;
                queue.pop();
                unsigned int indices[] = { 0,1,2,2,3,0 };
           

                IndexBuffer ibo(indices, 6);
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
