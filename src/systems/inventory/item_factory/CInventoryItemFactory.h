#pragma once

class CInventoryItemFactory
{
public:
	template <typename T>
	void RegisterItem(const std::string& name, std::function<std::unique_ptr<T>(int)> creator)
	{
		if (m_creators.find(name) != m_creators.end())
		{
			throw std::runtime_error("Item with name '" + name + "' is already registered.");
		}

		m_creators[name] = [creator](int ownerId) -> std::unique_ptr<InventoryItem> {
			return creator(ownerId);
		};

		m_registeredTypes[name] = typeid(T).hash_code();
	}

	template <typename T>
	std::unique_ptr<T> CreateInventoryItem(const std::string& name, int ownerId) const
	{
		auto it = m_creators.find(name);
		if (it != m_creators.end())
		{
			if (m_registeredTypes.at(name) != typeid(T).hash_code())
			{
				throw std::runtime_error("Type mismatch for item '" + name + "'.");
			}

			return std::unique_ptr<T>(static_cast<T*>(it->second(ownerId).release()));
		}
		throw std::runtime_error("Item '" + name + "' not found in the factory.");
	}

	~CInventoryItemFactory() = default;

private:
	std::unordered_map<std::string, std::function<std::unique_ptr<InventoryItem>(int)>> m_creators;
	std::unordered_map<std::string, size_t> m_registeredTypes;
};
