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
		Shaders shaderProgram;
		IndexBuffer ibo;
		VertexBufferLayout layout;
		Renderer renderer;
		unsigned int indices[16];
		std::priority_queue < 
			std::pair<Entity, int>, 
			std::vector<std::pair<Entity, int>>, 
			OrderInLayerComp> queue;
	public:
		RenderSystem() : indices{0,1,2,2,3,0}, ibo(indices, 6)
		{
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
				if (!texcomponent)
				{
					continue;
				}
				if (!texcomponent->exists)
				{
					continue;
				}
				queue.push({ entity, texcomponent->orderInLayer });

				

			}

		}

		std::array<float,4> ConvertToNdc(Transform* transform)
		{
			std::array<float, 4> convertedValues;
			convertedValues[0] = ((2 * transform->x) / VIRTUAL_WIDTH) - 1;
			convertedValues[1] = 1-((2 * transform->y) / VIRTUAL_HEIGHT);
			convertedValues[2] = ((2 * transform->width) / VIRTUAL_WIDTH);
			convertedValues[3] = -((2 * transform->height) / VIRTUAL_HEIGHT);
			return convertedValues;

		}

		std::array <float, 16> GetVertices(Transform* transform)
		{
			std::array<float,4>values = ConvertToNdc(transform);
			std::array<float,16> positions = {
			values[0],			values[1],			 0.0f,0.0f,
			values[0]+values[2],values[1],			 1.0f,0.0f,
			values[0],			values[1]+values[3], 1.0f,1.0f,
			values[0]+values[2],values[1]+values[3], 0.0f,1.0f
			};

			return positions;
		}


		void RenderQueue(EntityManager& em)
		{
			renderer.Clear();
			while (queue.size() > 0)
			{
				shaderProgram.SetUniform1i("u_Texture", 0);
				Entity entity = queue.top().first;
				queue.pop();
				TextureComponent* texcomponent = em.getComponent<TextureComponent>(entity);
				TextureLoader::BindTexture(texcomponent->name);
				Transform* transformcomponent = em.getComponent<Transform>(entity);
				std::array<float, 16> vertices = GetVertices(transformcomponent);
				VertexArray vao;
				VertexBuffer vbo{ vertices.data(),4 * 4 * sizeof(float) };
				vao.AddBuffer(vbo, layout);
				renderer.Draw(vao, ibo, shaderProgram);

			}
		}


	};

}
