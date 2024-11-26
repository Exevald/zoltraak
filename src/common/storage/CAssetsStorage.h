#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

template <typename AssetType>
class CAssetsStorage
{
public:
	static AssetType& GetAsset(const std::string& filePath)
	{
		auto it = m_assetsCache.find(filePath);
		if (it == m_assetsCache.end())
		{
			AssetType asset;
			if (!asset.loadFromFile(filePath))
			{
				throw std::runtime_error("Failed to load asset: " + filePath);
			}
			m_assetsCache[filePath] = std::move(asset);
		}
		return m_assetsCache[filePath];
	}

private:
	static std::unordered_map<std::string, AssetType> m_assetsCache;
};

template <typename AssetType>
std::unordered_map<std::string, AssetType> CAssetsStorage<AssetType>::m_assetsCache;