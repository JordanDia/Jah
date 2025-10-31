#pragma once

#include <unordered_map>
#include <typeindex>
#include <any>
#include <vector>

namespace Jah {

	using EntityID = uint32_t;
	
	class Registry
	{
	public:
		Registry() = default;

		uint32_t CreateEntity()
		{
			return m_NextEntityID++;
		}

		void DestroyEntity(EntityID entityID)
		{
			for (auto& [componentID, componentMap] : m_ComponentPools)
			{
				componentMap.erase(entityID);
			}
		}

		template<typename T, typename... Args>
		T& Add(EntityID entityID, Args&&... args)
		{
			auto componentID = std::type_index(typeid(T));
			auto& entityComponentMap = m_ComponentPools[componentID];

			entityComponentMap[entityID] = T(std::forward<Args>(args)...);
			return std::any_cast<T&>(entityComponentMap[entityID]);
		}

		template<typename T>
		T& Get(EntityID entityID)
		{
			auto componentID = std::type_index(typeid(T));

			auto it = m_ComponentPools.find(componentID);
			JAH_ASSERT(it != m_ComponentPools.end(), "Component not found in pool!");

			auto& entityComponentMap = it->second;
			auto entityIt = entityComponentMap.find(entityID);

			JAH_ASSERT(entityIt != entityComponentMap.end(), "Entity not found in component map!");
			return std::any_cast<T&>(entityIt->second);
		}

		template<typename T>
		bool Has(EntityID entityID)
		{
			auto typeID = std::type_index(typeid(T));

			auto it = m_ComponentPools.find(typeID);
			if (it == m_ComponentPools.end())
				return false;

			auto& entityComponentMap = it->second;
			return entityComponentMap.find(entityID) != entityComponentMap.end();
		}

		template<typename T>
		void Remove(EntityID entityID)
		{
			auto componentID = std::type_index(typeid(T));
			auto it = m_ComponentPools.find(componentID);
			if (it == m_ComponentPools.end())
				return;

			it->second.erase(entityID);
		}

		template<typename... Components>
		std::vector<EntityID> View()
		{
			std::vector<EntityID> result;

			if constexpr (sizeof...(Components) == 0)
				return result;

			using FirstComponent = std::tuple_element<0, std::tuple<Components...>>::type;
			auto& firstMap = m_ComponentPools[std::type_index(typeid(FirstComponent))];

			for (auto& [entityID, compAny] : firstMap)
			{
				bool hasAll = true;

				// Check other components
				([&] {
					auto it = m_ComponentPools.find(std::type_index(typeid(Components)));
					if (it == m_ComponentPools.end() || it->second.find(entityID) == it->second.end())
						hasAll = false;
					}(), ...); // fold expression over Components

				if (hasAll)
					result.push_back(entityID);
			}

			return result;
		}

	private:
		std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::any>> m_ComponentPools;
		uint32_t m_NextEntityID = 1;
	};
}