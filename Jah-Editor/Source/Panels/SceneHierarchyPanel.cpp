#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace Jah{

	SceneHierarchyPanel::SceneHierarchyPanel(const Shared<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Shared<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		/*ImGui::Begin("Scene Hierarchy");

		auto& entities = m_Context->GetRegistry().GetAllEntities();
		for (auto entity : entities)
		{
			auto& tag = m_Context->GetRegistry().Get<TagComponent>(entity);
			ImGui::Text("%s", tag.Name.c_str());
		}

		ImGui::End();*/
	}

}


