#include <Jah.h>
#include <Core/EntryPoint.h>
#include <iostream>
#include <print>
#include <memory>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Jah {

	class JahEditor : public Jah::Application
	{
	public:
		JahEditor()
			: Application("Jah Editor")
		{
			PushLayer(new EditorLayer());
		}

		~JahEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new JahEditor();
	}

}




