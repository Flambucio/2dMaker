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
		using EntityRegistry = std::unordered_set<Entity>;
		using EntityComponentSystemData = std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<Component>>>;
		using Entity_NameRegistry = std::unordered_map<std::string, Entity>;
		using NameRegistry = std::unordered_set<std::string>;
		using EntityName = std::string;
	private:

		EntityComponentSystemData entities;
		std::queue<int, std::deque<int>> availableIDs;
		
		EntityRegistry runtimeEntities;
		EntityRegistry virtualEntities;
		Entity nextID = 0;
		
	public:
		static const NameRegistry componentsTypesStr;
		EntityRegistry aliveEntities;
		Entity_NameRegistry entityNames;
		Entity cameraEntity = 0;

		inline bool nameAvailable(const EntityName &name) const
		{
			return (!(name == "")) && entityNames.find(name) == entityNames.end();
		}

		Entity createEntity(EntityName name, EntityType type = EntityType::NORMAL);
		void destroyEntityFromID(Entity entityID);
		void destroyEntity(EntityName name);
		bool renameEntity(const EntityName& oldName, const EntityName& newName);
		inline bool isAlive(Entity entityID) const
		{
			return aliveEntities.find(entityID) != aliveEntities.end();
		}

		inline bool isAlive(EntityName name) const 
		{
			if (entityNames.find(name) != entityNames.end()) return true;
			return false;
		}

		inline bool isRuntimeEntity(Entity entityID) const
		{
			return runtimeEntities.find(entityID) != runtimeEntities.end();
		}

		inline bool isVirtualEntity(Entity entityID) const
		{
			return virtualEntities.find(entityID) != virtualEntities.end();
		}

		

		template<typename T,typename...Args>
		bool addComponent(Entity entity, Args&&... args)
		{
			//TRACE("ADDING TO : ");
			//TRACE(entity);
			if (hasComponent<T>(entity))
			{
				WARN("Component already added");
				return false;
			}
			
			if (!HasDependences<T>(entity))
			{
				WARN("component " +std::string(typeid(T).name()) + " not added because of missing dependences");
				return false;
			}
			

			if constexpr (std::is_same_v<T, Script>)
			{
				if (!CheckFileComponentValidity(FileComponentLoader::SCRIPT, std::forward<Args>(args)...))
				{
					return false;
				}
			}
			else if constexpr (std::is_same_v<T, TextureComponent>)
			{
				if (!CheckFileComponentValidity(FileComponentLoader::TEXTURE, std::forward<Args>(args)...))
				{
					return false;
				}
			}
			else if constexpr (std::is_same_v < T, AudioComponent>)
			{
				if (!CheckFileComponentValidity(FileComponentLoader::AUDIO, std::forward<Args>(args)...))
				{
					return false;
				}
			}
			else if (std::is_same_v<T, Animation>)
			{
				if (!AllTexturesExist(std::forward<Args>(args)...))
				{
					return false;
				}
			}
			else if constexpr (std::is_same_v<T, Camera>)
			{
				if (CheckCameraComponentExistence() || isVirtualEntity(entity))
				{
					return false;
				}
				cameraEntity = entity;
			}
			
			
			
			entities[entity][std::type_index(typeid(T))] = std::make_unique<T>(std::forward<Args>(args)...);
			return true;
			//TRACE("added component:");
			//TRACE(typeid(T).name());
		}

		

		template<typename T>
		bool RemoveComponent(Entity entity)
		{
			auto entityIt = entities.find(entity);
			if (entityIt == entities.end())
			{
				WARN("Entity not found");
				return false;
			}

			auto& componentMap = entityIt->second;
			auto compIt = componentMap.find(std::type_index(typeid(T)));

			if (compIt != componentMap.end())
			{
				if (!canSafelyRemove<T>(entity))
				{
					return false;
				}
				componentMap.erase(compIt);
				//TRACE("Removed component:");
				//TRACE(typeid(T).name());
			}

			return true;

		}

		inline bool CheckCameraComponentExistence()
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

		inline EntityComponentSystemData& GetMap()
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
