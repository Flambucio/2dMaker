#pragma once

#include "../common/Core.h"
#include "Components.h"

namespace D2Maker
{
	enum class FileComponentLoader
	{
		AUDIO = 0,
		TEXTURE = 1,
		SCRIPT = 2 ,
	};
	enum class EntityType
	{
		VIRTUAL=-1,
		NORMAL=0,
		RUNTIME=1,
	};
	using Entity = uint32_t;
	class EntityManager
	{
	private:
		std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<Component>>> entities;
		std::queue<int, std::deque<int>> availableIDs;
		std::unordered_set<Entity> runtimeEntities;
		std::unordered_set<Entity> virtualEntities;
		Entity nextID = 0;
	public:
		std::unordered_set<Entity> aliveEntities;
		Entity createEntity(EntityType type=EntityType::NORMAL)
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
			if (type == EntityType::VIRTUAL)
			{
				virtualEntities.insert(id);
			}
			else if (type == EntityType::RUNTIME)
			{
				runtimeEntities.insert(id);
			}
			return id;
		}

		void destroyEntity(Entity entityID)
		{
			entities.erase(entityID);
			if (isRuntimeEntity(entityID))
			{
				runtimeEntities.erase(entityID);
			}
			else if (isVirtualEntity(entityID))
			{
				virtualEntities.erase(entityID);
			}
			availableIDs.push(entityID);
			aliveEntities.erase(entityID);
		}

		bool isAlive(Entity entityID) const
		{
			return aliveEntities.find(entityID) != aliveEntities.end();
		}

		bool isRuntimeEntity(Entity entityID) const
		{
			return runtimeEntities.find(entityID) != runtimeEntities.end();
		}

		bool isVirtualEntity(Entity entityID) const
		{
			return virtualEntities.find(entityID) != virtualEntities.end();
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
			
			if (!HasDependences<T>(entity))
			{
				WARN("component " +std::string(typeid(T).name()) + " not added because of missing dependences");
				return;
			}
			

			if constexpr (std::is_same_v<T, Script>)
			{
				if (!CheckFileComponentValidity(FileComponentLoader::SCRIPT, std::forward<Args>(args)...))
				{
					return;
				}
			}
			else if constexpr (std::is_same_v<T, TextureComponent>)
			{
				if (!CheckFileComponentValidity(FileComponentLoader::TEXTURE, std::forward<Args>(args)...))
				{
					return;
				}
			}
			else if constexpr (std::is_same_v < T, Audio>)
			{
				if (!CheckFileComponentValidity(FileComponentLoader::AUDIO, std::forward<Args>(args)...))
				{
					return;
				}
			}
			
			
			entities[entity][std::type_index(typeid(T))] = std::make_unique<T>(std::forward<Args>(args)...);
			TRACE("added component:");
			TRACE(typeid(T).name());
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			auto entityIt = entities.find(entity);
			if (entityIt == entities.end())
			{
				WARN("Entity not found");
				return;
			}

			auto& componentMap = entityIt->second;
			auto compIt = componentMap.find(std::type_index(typeid(T)));

			if (compIt != componentMap.end())
			{
				if (!canSafelyRemove<T>(entity))
				{
					return;
				}
				componentMap.erase(compIt);
				TRACE("Removed component:");
				TRACE(typeid(T).name());
			}

		}

		template<typename...Args>
		bool CheckFileComponentValidity(const FileComponentLoader &loader,Args&&...args)
		{
			if constexpr (sizeof...(Args) > 0)
			{
				auto tup = std::forward_as_tuple(args...);
				using FirstArgType = std::tuple_element_t<0, decltype(tup)>;
				if constexpr (std::is_convertible_v<FirstArgType, std::string>)
				{
					const std::string& name = std::get<0>(tup);
					switch (loader)
					{
					case FileComponentLoader::AUDIO:
						if (!AudioLoader::Exists(name))
						{
							WARN("Audio not loaded: " + name);
							return false;
						}
						break;
					case FileComponentLoader::TEXTURE:
						if (!TextureLoader::Exists(name))
						{
							WARN("Texture not loaded: " + name);
							return false;
						}
						break;
					case FileComponentLoader::SCRIPT:
						if (!(std::filesystem::exists(name) && std::filesystem::is_regular_file(name)))
						{
							WARN("Script file does not exist: " + name);
							return false;
						}
						break;
					}
					
					
					

				}
				else
				{
					WARN("Audio requires a name as first argument");
					return false;
				}
			}
			else
			{
				WARN("Audio component requires at least one argument (name)");
				return false;
			}

			return true;
		}

		template<typename T>
		bool HasDependences(Entity entity)
		{
			if constexpr (std::is_same_v<T, Velocity> || std::is_same_v<T, Collider> || std::is_same_v<T, TextureComponent>) 
			{
				return hasComponent<Transform>(entity);
			}
			else if constexpr (std::is_same_v<T, RigidBody>) 
			{
				return hasComponent<Transform>(entity) && hasComponent<Velocity>(entity) && hasComponent<Collider>(entity);
			}
			return true;
		}

		template<typename T>
		bool canSafelyRemove(Entity entity)
		{
			if constexpr (std::is_same_v<T, Transform>) 
			{
				return !(hasComponent<Velocity>(entity) || hasComponent<RigidBody>(entity) || 
					hasComponent<Collider>(entity) || hasComponent<TextureComponent>(entity));
			}
			else if constexpr (std::is_same_v<T, Velocity> || std::is_same_v<T, Collider>)
			{
				return !hasComponent<RigidBody>(entity);
			}
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

		//deprecated
		template<typename T,typename U>
		bool isComponent()
		{
			return std::type_index(typeid(T)) == std::type_index(typeid(U));

		}

	};
}
