#pragma once
#include "Core/Core.h"
#include "Scene.h"
#include <cstdint>
#include <memory>

#include "Components.h"

namespace Jah {

	using EntityID = uint32_t;

	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityID id, Scene* scene)
			: m_ID(id), m_Scene(scene) {}	

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_Scene->GetRegistry().Add<T>(m_ID, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(m_ID, component);
			return component;
		}
		
		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->GetRegistry().AddOrReplace<T>(m_ID, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(m_ID, component);
			return component;
		}

		template<typename T>
		T& GetComponent() const
		{
			return m_Scene->GetRegistry().Get<T>(m_ID);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->GetRegistry().Has<T>(m_ID);
		}

		template<typename T>
		void RemoveComponent()
		{
			return m_Scene->GetRegistry().Remove<T>(m_ID);
		}

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() const { return GetComponent<TagComponent>().Name; }

		EntityID GetID() const { return m_ID; }
		operator uint32_t() const { return (uint32_t)m_ID; }

		bool operator==(const Entity& other) const { return m_ID == other.m_ID; }
		bool operator!=(const Entity& other) const { return !(*this == other); }


		
	private:
		EntityID m_ID = 0;
		Scene* m_Scene = nullptr;
	};

	

}