#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Renderer/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Jah {

	

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

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
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