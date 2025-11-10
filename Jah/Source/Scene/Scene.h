#pragma once

#include "Registry.h"
#include "Core/Timestep.h"
#include "Renderer/EditorCamera.h"

#include <string>

namespace Jah {

	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(EntityID entityID);

		void OnRender(Camera& camera);
		void OnRender(OrthographicCamera& camera);
		void OnUpdateEditor(Timestep timestep, EditorCamera& camera);
		void OnUpdateRuntime(Timestep timestep);

		void OnViewportResize(uint32_t width, uint32_t height);

		Registry& GetRegistry() { return m_Registry; }

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(EntityID entity, T& component);

	private:
		Registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
	};

}