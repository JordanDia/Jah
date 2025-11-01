#pragma once

#include "Scene/Scene.h"
#include "Core/Core.h"
#include "Core/Log.h"

namespace Jah {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Shared<Scene>& context);

		void SetContext(const Shared<Scene>& context);

		void OnImGuiRender();
	private:
		Shared<Scene> m_Context;
	};

}