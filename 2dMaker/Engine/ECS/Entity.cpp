#include "Entity.h"
using namespace D2Maker;

Entity EntityManager::createEntity(EntityName name, EntityType type)
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



void EntityManager::destroyEntityFromID(Entity entityID)
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

void EntityManager::destroyEntity(EntityName name)
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

bool EntityManager::renameEntity(const EntityName& oldName, const EntityName& newName)
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