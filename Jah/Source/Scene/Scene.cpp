#include "jahpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Registry.h"

#include "Renderer/Renderer2D.h"


namespace Jah {

	Entity Scene::CreateEntity()
	{
		return {m_Registry.CreateEntity(), this};
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.DestroyEntity(entity.GetID());
	}

	void Scene::OnUpdate(Timestep timestep, OrthographicCamera& camera)
	{

		Renderer2D::BeginScene(camera);
		auto view = m_Registry.View<TransformComponent>();

		for (auto entityID : view)
		{
			auto& transform = m_Registry.Get<TransformComponent>(entityID);
			auto& spriteRenderer = m_Registry.Get<SpriteRendererComponent>(entityID);
			Renderer2D::DrawRotatedQuad(transform.Position, transform.Size, transform.Rotation, spriteRenderer.Color);
		}

		Renderer2D::EndScene();
	}

}