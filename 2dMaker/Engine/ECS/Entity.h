#pragma once

#include "../common/Core.h"
#include "Components.h"

namespace D2Maker
{
	using Entity = uint32_t;
	class EntityManager
	{
	private:
		std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<Component>>> entities;
		std::queue<int, std::vector<int>> availableIDs;
		std::unordered_set<Entity> aliveEntities;
		Entity nextID = 0;

	public:
		Entity createEntity()
		{
			Entity id;
			if (!availableIDs.empty())
			{
				id = availableIDs.front();
				availableIDs.pop();
			}
			else
			{
				id = nextID;
				nextID++;
			}
			aliveEntities.insert(id);
			return id;
		}

		void destroyEntity(Entity entityID)
		{
			entities.erase(entityID);
			availableIDs.push(entityID);
			aliveEntities.erase(entityID);
		}

		bool isAlive(Entity entityID) const
		{
			return aliveEntities.find(entityID) != aliveEntities.end();
		}

		template<typename T,typename...Args>
		void addComponent(Entity entity, T component)
		{
			entities[entity][std::type_index(typeid(T))] = std::make_unique<T>(std::forward<Args>(args)...);
		}


		template<typename T>
		T* getComponent(Entity entity,T component)
		{
			auto it = entities[entity].find(std::type_index(typeid(T)));
			if (it != entities.end())
			{
				return static_cast<T*>(it->second.get());
			}
			return nullptr;
		}

		template <typename T>
		bool HasComponent(Entity entity) {
			return entities[entity].find(std::type_index(typeid(T))) != entities[entity].end();
		}
	};
}
