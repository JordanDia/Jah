#include "SceneHierarchyPanel.h"
#include "Scene/Components.h"
#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

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
		ImGui::Begin("Scene Hierarchy");

		auto& entities = m_Context->GetRegistry().GetEntities();
		for (auto entityID : entities)
		{
			DrawEntityNode(entityID);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = 0;
		}

		ImGui::End();


		ImGui::Begin("Properties");

		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(EntityID entityID)
	{
		auto& tag = m_Context->GetRegistry().Get<TagComponent>(entityID);
		
		ImGuiTreeNodeFlags flags = (m_SelectionContext == entityID ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)entityID, flags, tag.Name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entityID;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = (m_SelectionContext == entityID ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)12983198, flags, tag.Name.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

	}

	void SceneHierarchyPanel::DrawComponents(EntityID entityID)
	{
		auto& registry = m_Context->GetRegistry();
		
		if (registry.Has<TagComponent>(entityID))
		{
			auto& tag = registry.Get<TagComponent>(entityID).Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (registry.Has<TransformComponent>(entityID))
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform"))
			{
				auto& transform = registry.Get<TransformComponent>(entityID);
				glm::vec3& translation = transform.Translation;

				ImGui::DragFloat3("Position", glm::value_ptr(translation), 0.05f);

				ImGui::TreePop();

			}
			
		}
	}

}


