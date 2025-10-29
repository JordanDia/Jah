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

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep timestep, OrthographicCamera& camera);

		Registry& GetRegistry() { return m_Registry; }

	private:
		Registry m_Registry;
	};

}