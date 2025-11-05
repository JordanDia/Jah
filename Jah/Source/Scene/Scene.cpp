#include "jahpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Registry.h"

#include "Renderer/Renderer2D.h"


namespace Jah {

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.CreateEntity(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(EntityID entityID)
	{
		m_Registry.DestroyEntity(entityID);
	}

	void Scene::OnRender(OrthographicCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		
		{
			auto view = m_Registry.View<TransformComponent, SpriteRendererComponent>();

			for (auto entityID : view)
			{
				auto& transform = m_Registry.Get<TransformComponent>(entityID);
				auto& spriteRenderer = m_Registry.Get<SpriteRendererComponent>(entityID);

				Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer);
			}

		}

		Renderer2D::EndScene();
	}

	void Scene::OnRender(Camera& camera)
	{

		


		
	}

	void Scene::OnUpdate(Timestep timestep)
	{
		{
			auto view = m_Registry.View<NativeScriptComponent>();

			for (auto entityID : view)
			{
				auto& nsc = m_Registry.Get<NativeScriptComponent>(entityID);
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entityID, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(timestep);


			}
		}


		{

			auto view = m_Registry.View<TransformComponent, SpriteRendererComponent>();

			for (auto entityID : view)
			{
				auto& transform = m_Registry.Get<TransformComponent>(entityID);

				if (m_Registry.Has<TagComponent>(entityID))
				{
					auto& tag = m_Registry.Get<TagComponent>(entityID);
					if (tag.Name == "Grass Sprite")
					{
						static float rotationZ = 0.0f;
						rotationZ += timestep * 0.5f;

						transform.Rotation = glm::vec3(0.0f, 0.0f, rotationZ);
					}
				}
			}
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.View<TransformComponent, CameraComponent>();

			for (auto entityID : view)
			{
				if (!m_Registry.Has<CameraComponent>(entityID) || !m_Registry.Has<TransformComponent>(entityID))
					continue;

				auto& transformComponent = m_Registry.Get<TransformComponent>(entityID);
				auto& cameraComponent = m_Registry.Get<CameraComponent>(entityID);

				if (cameraComponent.Primary)
				{
					mainCamera = &cameraComponent.Camera;
					cameraTransform = transformComponent.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{

			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			{
				auto view = m_Registry.View<TransformComponent, SpriteRendererComponent>();

				for (auto entityID : view)
				{

					auto& transform = m_Registry.Get<TransformComponent>(entityID);
					auto& spriteRenderer = m_Registry.Get<SpriteRendererComponent>(entityID);

					Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer);
				}

			}

			Renderer2D::EndScene();
		}

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.View<CameraComponent>();
		for (auto entityID : view)
		{
			auto& cameraComponent = m_Registry.Get<CameraComponent>(entityID);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(EntityID entityID, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(EntityID entityID, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(EntityID entityID, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(EntityID entityID, SpriteRendererComponent& component)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(EntityID entityID, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(EntityID entityID, NativeScriptComponent& component)
	{

	}
}