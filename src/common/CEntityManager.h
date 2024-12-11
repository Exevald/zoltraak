#pragma once

#include "Components.h"
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

using EntityId = int;

class CEntityManager
{
public:
	static CEntityManager& GetInstance()
	{
		static CEntityManager instance;
		return instance;
	}

	EntityId CreateEntity()
	{
		EntityId newId = m_nextId++;
		m_entities.insert(newId);
		return newId;
	}

	void DeleteEntity(EntityId entityId)
	{
		m_entities.erase(entityId);
		for (auto& [type, componentMap] : m_components)
		{
			componentMap.erase(entityId);
		}
	}

	template <typename ComponentType, typename... Args>
	void AddComponent(EntityId entityId, Args&&... args)
	{
		auto& componentMap = GetComponentMap<ComponentType>();
		componentMap.emplace(entityId, ComponentType(std::forward<Args>(args)...));
	}

	template <typename ComponentType>
	void RemoveComponent(EntityId entityId)
	{
		auto& componentMap = GetComponentMap<ComponentType>();
		componentMap.erase(entityId);
	}

	std::vector<EntityId> GetAllEntities() const
	{
		return { std::vector<EntityId>(m_entities.begin(), m_entities.end()) };
	}

	template <typename ComponentType>
	ComponentType* GetComponent(EntityId entityId)
	{
		auto& componentMap = GetComponentMap<ComponentType>();
		auto it = componentMap.find(entityId);
		return (it != componentMap.end()) ? &it->second : nullptr;
	}

	template <typename... Components>
	std::vector<EntityId> GetEntitiesWithComponents()
	{
		std::vector<EntityId> entities;
		for (EntityId entityId : m_entities)
		{
			if ((HasComponent<Components>(entityId) && ...))
			{
				entities.push_back(entityId);
			}
		}
		return entities;
	}

	template <typename ComponentType>
	bool HasComponent(EntityId entityId)
	{
		auto& componentMap = GetComponentMap<ComponentType>();
		return componentMap.find(entityId) != componentMap.end();
	}

	CEntityManager(const CEntityManager&) = delete;
	CEntityManager& operator=(const CEntityManager&) = delete;

private:
	CEntityManager()
		: m_nextId(0)
	{
	}

	template <typename ComponentType>
	std::unordered_map<EntityId, ComponentType>& GetComponentMap()
	{
		static std::unordered_map<EntityId, ComponentType> componentMap;
		return componentMap;
	}

	EntityId m_nextId;
	std::unordered_set<EntityId> m_entities;
	std::unordered_map<std::type_index, std::unordered_map<EntityId, std::unique_ptr<IComponent>>> m_components;
};
