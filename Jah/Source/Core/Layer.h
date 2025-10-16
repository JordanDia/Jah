#pragma once

#include "Events/Event.h"
#include "Timestep.h"

namespace Jah {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_Name; }

	private:
		std::string m_Name;
	};

}