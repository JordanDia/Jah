#pragma once
#include "Core/Core.h"
#include "Scene.h"
#include <cstdint>
#include <memory>

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
			return m_Scene->GetRegistry().Add<T>(m_ID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{

		}

		template<typename T>
		T& HasComponent()
		{

		}

		template<typename T>
		void RemoveComponent()
		{

		}

		EntityID GetID() const { return m_ID; }

		template<typename T>
		bool HasComponent();

		bool operator==(const Entity& other) const { return m_ID == other.m_ID; }
		bool operator!=(const Entity& other) const { return !(*this == other); }
		
	private:
		EntityID m_ID = 0;
		Scene* m_Scene = nullptr;
	};

	

}