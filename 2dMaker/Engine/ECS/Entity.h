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
		ANIMATION=3
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
		static const std::unordered_set<std::string> componentsTypesStr;
		std::unordered_set<Entity> aliveEntities;
		std::unordered_map<std::string, Entity> entityNames;
		Entity cameraEntity = 0;

		bool nameAvailable(const std::string &name) const
		{
			return (!(name == "")) && entityNames.find(name) == entityNames.end();
		}

		Entity createEntity(std::string name, EntityType type = EntityType::NORMAL)
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
			entityNames[name] = id;
			return id;
		}
		
		void destroyEntityFromID(Entity entityID)
		{
			for (auto it = entityNames.begin(); it != entityNames.end(); ++it)
			{
				if (it->second == entityID)
				{
					entityNames.erase(it);
					break;
				}
			}
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

		void destroyEntity(std::string name)
		{
			auto it = entityNames.find(name);
			if (it == entityNames.end()) return;
			Entity entity = it->second;
			entities.erase(entity);
			entityNames.erase(name);
			if (isRuntimeEntity(entity))
			{
				runtimeEntities.erase(entity);
			}
			else if (isVirtualEntity(entity))
			{
				virtualEntities.erase(entity);
			}
		}

		bool renameEntity(const std::string& oldName,const std::string& newName)
		{
			auto it = entityNames.find(oldName);
			if (it == entityNames.end())
			{
				WARN("Entity with name:" + oldName + " does not exist");
				return false;
			}
			if (!nameAvailable(newName))
			{
				WARN("Entity name:" + newName + " is already taken or invalid.");
				return false;
			}

			Entity entityID = it->second;
			entityNames.erase(it);
			entityNames[newName] = entityID;
			return true;

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
			//TRACE("ADDING TO : ");
			//TRACE(entity);
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
			else if (std::is_same_v<T, Animation>)
			{
				if (!AllTexturesExist(std::forward<Args>(args)...))
				{
					return;
				}
			}
			else if constexpr (std::is_same_v<T, Camera>)
			{
				if (CheckCameraComponentExistence() || isVirtualEntity(entity))
				{
					return;
				}
				cameraEntity = entity;
			}
			
			
			
			entities[entity][std::type_index(typeid(T))] = std::make_unique<T>(std::forward<Args>(args)...);
			//TRACE("added component:");
			//TRACE(typeid(T).name());
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
				//TRACE("Removed component:");
				//TRACE(typeid(T).name());
			}

		}

		bool CheckCameraComponentExistence()
		{
			for (Entity entity : aliveEntities)
			{
				if (hasComponent<Camera>(entity))
				{
					return true;
				}
			}
			return false;
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
					WARN("Unsupported argument type for file component");
					return false;
				}
			}
			else
			{
				WARN("Component requires at least one argument");
				return false;
			}

			return true;
		}

		template<typename... Args>
		bool AllTexturesExist(Args&&...args)
		{
			if constexpr (sizeof...(Args) > 0)
			{
				auto tup = std::forward_as_tuple(args...);
				using FirstArgType = std::tuple_element_t<0, decltype(tup)>;
				using CleanedType = std::decay_t<FirstArgType>;

				if constexpr (std::is_same_v<CleanedType, std::vector<std::string>>)
				{
					const auto& names = std::get<0>(tup);
					for (const std::string& name : names)
					{
						if (!CheckFileComponentValidity(FileComponentLoader::TEXTURE, name))
						{
							return false;
						}
					}
					return true;
				}
				else
				{
					WARN("Invalid arguments - expected vector<string>");
					return false;
				}
			}
			else
			{
				WARN("Missing arguments - animation check");
				return false;
			}
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
			if constexpr (std::is_same_v<T, Camera>)
			{
				return hasComponent<Transform>(entity);
			}
			if constexpr (std::is_same_v<T, Follow>)
			{
				return hasComponent<Velocity>(entity);
			}
			return true;
		}

		template<typename T>
		bool canSafelyRemove(Entity entity)
		{
			if constexpr (std::is_same_v<T, Transform>) 
			{
				return !(hasComponent<Velocity>(entity) || hasComponent<RigidBody>(entity) || 
					hasComponent<Collider>(entity) || hasComponent<TextureComponent>(entity) ||
					hasComponent<Camera>(entity));
			}
			else if constexpr (std::is_same_v<T, Velocity> || std::is_same_v<T, Collider>)
			{
				return !hasComponent<RigidBody>(entity);
			}
			else if constexpr (std::is_same_v<T, Follow>)
			{
				return !hasComponent<Velocity>(entity);
			}

			return true;
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

		std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<Component>>>& GetMap()
		{
			return entities;
		}

		

		//deprecated
		template<typename T,typename U>
		bool isComponent()
		{
			return std::type_index(typeid(T)) == std::type_index(typeid(U));

		}

	};
}
