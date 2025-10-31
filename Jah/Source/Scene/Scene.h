#pragma once

#include "Registry.h"
#include "Core/Timestep.h"
#include "Renderer/Camera.h"

namespace Jah {

	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRender(Camera& camera);
		void OnRender(OrthographicCamera& camera);
		void OnUpdate(Timestep timestep);

		void OnViewportResize(uint32_t width, uint32_t height);

		Registry& GetRegistry() { return m_Registry; }

	private:
		Registry m_Registry;
		uint32_t m_ViewportWidth;
		uint32_t m_ViewportHeight;
	};

}