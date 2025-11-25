#pragma once

#include <glm/glm.hpp>
#include "Renderer/Camera.h"
#include "Core/UUID.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <glm/gtx/quaternion.hpp>
#include <box2d/box2d.h>

namespace Jah {
	
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		IDComponent(UUID id)
			: ID(id) {}
	};

	struct TagComponent
	{
		std::string Name;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& name)
			: Name(name) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };


		TransformComponent() = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: Translation(position), Rotation(rotation), Scale(scale) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color	{ 0.3f, 0.4f, 1.0f, 1.0f };

		Shared<Texture2D> Texture = nullptr;
		glm::vec2 TexCoordMin = { 0.0f, 0.0f };
		glm::vec2 TexCoordMax = { 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4 color)
			: Color(color) {}
		SpriteRendererComponent(Shared<Texture2D> texture, const glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
			: Texture(texture), Color(color) {
		}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class ScriptableEntity;
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		using InstantiateFn = ScriptableEntity * (*)();
		using DestroyFn = void (*)(NativeScriptComponent*);

		InstantiateFn InstantiateScript = nullptr;
		DestroyFn DestroyScript = nullptr;

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() -> ScriptableEntity* { return new T(); };
			DestroyScript = [](NativeScriptComponent* nsc)
				{
					delete static_cast<T*>(nsc->Instance);
					nsc->Instance = nullptr;
				};
		}
	};

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;

		// Storage for runtime
		b2BodyId RuntimeBodyID = b2_nullBodyId;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};


	struct BoxCollider2DComponent
	{

		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};












	static std::pair<glm::vec2, glm::vec2> GetTexCoords(
		const glm::vec2& coords,
		const glm::vec2& cellSize,
		const glm::vec2& textureSize)
	{
		glm::vec2 min = {
			(coords.x * cellSize.x) / textureSize.x,
			(coords.y * cellSize.y) / textureSize.y
		};

		glm::vec2 max = {
			((coords.x + 1) * cellSize.x) / textureSize.x,
			((coords.y + 1) * cellSize.y) / textureSize.y
		};

		return { min, max };
	}
}