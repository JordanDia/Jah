#pragma once

#include "Core/Core.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace Jah {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Shared<Scene>& context);

		void SetContext(const Shared<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(EntityID entityID);
		void DrawComponents(EntityID entityID);
	private:
		Shared<Scene> m_Context;
		EntityID m_SelectionContext = 0;
	};

}