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
		std::queue<int, std::deque<int>> availableIDs;
		Entity nextID = 0;
	public:
		std::unordered_set<Entity> aliveEntities;
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
		void addComponent(Entity entity, Args&&... args)
		{
			TRACE("ADDING TO : ");
			TRACE(entity);
			if (hasComponent<T>(entity))
			{
				WARN("Component already added");
				return;
			}
			if (isComponent<T, Velocity>() || isComponent<T, Collider>() || isComponent<T, RigidBody>() ||
				isComponent<T,TextureComponent>())
			{
				if (!hasComponent<Transform>(entity))
				{
					TRACE(typeid(T).name());
					WARN("not added because of missing transform component");
					return;
				}	
				if (isComponent<T, RigidBody>() && (!hasComponent<Velocity>(entity) || !hasComponent<Collider>(entity)))
				{
					WARN("RigidBody not added because of missing Velocity/Collider component/s");
					return;
				}
			}
			entities[entity][std::type_index(typeid(T))] = std::make_unique<T>(std::forward<Args>(args)...);
			TRACE("added component:");
			TRACE(typeid(T).name());
		}

		

		template<typename T>
		T* getComponent(Entity entity)
		{
			auto it = entities[entity].find(std::type_index(typeid(T)));
			if (it != entities[entity].end())
			{
				return static_cast<T*>(it->second.get());
			}
			
			return nullptr;
		}

		template <typename T>
		bool hasComponent(Entity entity) 
		{
			return entities[entity].find(std::type_index(typeid(T))) != entities[entity].end();
		}

		template<typename T,typename U>
		bool isComponent()
		{
			return std::type_index(typeid(T)) == std::type_index(typeid(U));

		}

	};
}
