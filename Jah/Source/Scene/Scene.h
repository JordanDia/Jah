#pragma once

#include "Registry.h"
#include "Core/Timestep.h"
#include "Renderer/EditorCamera.h"
#include "Core/UUID.h"

#include <string>
#include <box2d/id.h>

namespace Jah {

	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		static Shared<Scene> Copy(Shared<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(EntityID entityID);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnRender(Camera& camera);
		void OnRender(OrthographicCamera& camera);
		void OnUpdateEditor(Timestep timestep, EditorCamera& camera);
		void OnUpdateRuntime(Timestep timestep);

		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		Registry& GetRegistry() { return m_Registry; }

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllComponentsWith()
		{
			return m_Registry.View<Components...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(EntityID entity, T& component);

	private:
		Registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		b2WorldId m_PhysicsWorldID{};

		friend class Entity;
		friend class SceneSerializer;
	};

}