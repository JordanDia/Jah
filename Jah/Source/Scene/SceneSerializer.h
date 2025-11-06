#pragma once


#include "Scene.h"
#include "Core/Core.h"
#include <filesystem>


namespace Jah {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Shared<Scene>& scene);

		void Serialize(const std::filesystem::path& filepath);
		void SerializeRuntime(const std::filesystem::path& filepath);

		bool Deserialize(const std::filesystem::path& filepath);
		bool DeserializeRuntime(const std::filesystem::path& filepath);

	private:
		Shared<Scene> m_Scene;

	};

}
