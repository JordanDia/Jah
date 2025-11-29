#include "jahpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Registry.h"
#include "Core/UUID.h"

#include "Renderer/Renderer2D.h"
#include "ScriptableEntity.h"

#include <box2d/box2d.h>
#include <cmath>
#include <unordered_map>

namespace Jah {


	static b2BodyType Rigidbody2DTypeToBox2DType(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
		case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		default: JAH_ASSERT(false, "Invalid body type!"); return b2_staticBody;
		}
	}

	template<typename Component>
	static void CopyComponent(Registry& dst, Registry& src, const std::unordered_map<UUID, EntityID>& entityMap)
	{
		auto view = src.View<Component>();
		for (auto entityID : view)
		{
			UUID uuid = src.Get<IDComponent>(entityID).ID;
			JAH_ASSERT(entityMap.find(uuid) != entityMap.end(), "Entity not found in entity map!");
			auto dstEntityID = entityMap.at(uuid);

			auto& component = src.Get<Component>(entityID);
			dst.Add<Component>(dstEntityID, component);
		}
	}


	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	}


	Shared<Scene> Scene::Copy(Shared<Scene> other)
	{
		Shared<Scene> newScene = CreateShared<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, EntityID> entityMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.View<IDComponent>();
		for (auto entityID : idView)
		{
			UUID uuid = srcSceneRegistry.Get<IDComponent>(entityID).ID;
			const auto& name = srcSceneRegistry.Get<TagComponent>(entityID).Name;

			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			entityMap[uuid] = newEntity.GetID();
		}

		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);
		CopyComponent<CircleRendererComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);
		CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);
		CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);
		CopyComponent<CircleCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, entityMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name /*= std::string()*/)
	{
		Entity entity = { m_Registry.CreateEntity(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(EntityID entityID)
	{
		m_Registry.DestroyEntity(entityID);
	}

	void Scene::OnRuntimeStart()
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { 0.0f, -9.8f };
		m_PhysicsWorldID = b2CreateWorld(&worldDef);

		auto view = m_Registry.View<Rigidbody2DComponent>();
		for (auto entityID : view)
		{
			Entity entity = { entityID, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = Rigidbody2DTypeToBox2DType(rb2d.Type);
			bodyDef.position = { transform.Translation.x, transform.Translation.y };
			
			bodyDef.rotation = b2MakeRot(transform.Rotation.z);
			
			b2BodyId bodyID = b2CreateBody(m_PhysicsWorldID, &bodyDef);
			rb2d.RuntimeBodyID = bodyID;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2Polygon boxShape = b2MakeBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.material.friction = bc2d.Friction;
				shapeDef.material.restitution = bc2d.Restitution;
				shapeDef.density = 1.0f;

				b2CreatePolygonShape(bodyID, &shapeDef, &boxShape);


				
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				
				b2Circle circle;
				circle.center = { cc2d.Offset.x * transform.Scale.x, cc2d.Offset.y * transform.Scale.y };
				circle.radius = cc2d.Radius * transform.Scale.x;

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.material.friction = cc2d.Friction;
				shapeDef.material.restitution = cc2d.Restitution;
				shapeDef.density = 1.0f;

				b2CreateCircleShape(bodyID, &shapeDef, &circle);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		b2DestroyWorld(m_PhysicsWorldID);
		m_PhysicsWorldID = b2_nullWorldId;
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

				Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer, (int)entityID);
			}

		}

		{
			auto view = m_Registry.View<TransformComponent, CircleRendererComponent>();

			for (auto entityID : view)
			{

				auto& transform = m_Registry.Get<TransformComponent>(entityID);
				auto& circleRenderer = m_Registry.Get<CircleRendererComponent>(entityID);

				Renderer2D::DrawCircle(transform.GetTransform(), circleRenderer.Color, circleRenderer.Thickness, circleRenderer.Fade, (int)entityID);
			}

		}

		Renderer2D::EndScene();
	}

	void Scene::OnRender(Camera& camera)
	{}

	void Scene::OnUpdateEditor(Timestep timestep, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		
		// Draw Sprites
		{
			auto view = m_Registry.View<TransformComponent, SpriteRendererComponent>();

			for (auto entityID : view)
			{

				auto& transform = m_Registry.Get<TransformComponent>(entityID);
				auto& spriteRenderer = m_Registry.Get<SpriteRendererComponent>(entityID);

				Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer, (int)entityID);
				//Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f), (int)entityID);
			}

		}

		// Draw Circles
		{
			auto view = m_Registry.View<TransformComponent, CircleRendererComponent>();

			for (auto entityID : view)
			{

				auto& transform = m_Registry.Get<TransformComponent>(entityID);
				auto& circleRenderer = m_Registry.Get<CircleRendererComponent>(entityID);

				Renderer2D::DrawCircle(transform.GetTransform(), circleRenderer.Color, circleRenderer.Thickness, circleRenderer.Fade, (int)entityID);
			}

		}
		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep timestep)
	{

		// Update scripts
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

		// Physics
		{
			int32_t subStepCount = 4;
			b2World_Step(m_PhysicsWorldID, timestep, subStepCount);

			// Retrieve transform from Box2D
			auto view = m_Registry.View<Rigidbody2DComponent>();
			for (auto entityID : view)
			{
				Entity entity{ entityID, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2BodyId bodyID = rb2d.RuntimeBodyID;

				const auto& position = b2Body_GetPosition(bodyID);
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;

				b2Rot rotation = b2Body_GetRotation(bodyID);
				float angle = std::atan2(rotation.s, rotation.c);
				transform.Rotation.z = angle;
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

					Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer, (int)entityID);
				}
			}

			{
				auto view = m_Registry.View<TransformComponent, CircleRendererComponent>();

				for (auto entityID : view)
				{
					auto& transform = m_Registry.Get<TransformComponent>(entityID);
					auto& circleRenderer = m_Registry.Get<CircleRendererComponent>(entityID);

					Renderer2D::DrawCircle(transform.GetTransform(), circleRenderer.Color, circleRenderer.Thickness, circleRenderer.Fade, (int)entityID);
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

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName(); // copy to avoid possible dangling reference if registry resizes
		Entity newEntity = CreateEntity(name);

		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CircleRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<CircleCollider2DComponent>(newEntity, entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.View<CameraComponent>();
		for (auto entityID : view)
		{
			const auto& camera = m_Registry.Get<CameraComponent>(entityID);
			if (camera.Primary)
				return { entityID, this };
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(EntityID entityID, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(EntityID entityID, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(EntityID entityID, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(EntityID entityID, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(EntityID entityID, SpriteRendererComponent& component)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(EntityID entityID, CircleRendererComponent& component)
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

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(EntityID entityID, Rigidbody2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(EntityID entityID, BoxCollider2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(EntityID entityID, CircleCollider2DComponent& component)
	{

	}
}