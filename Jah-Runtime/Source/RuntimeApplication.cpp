#include <Jah.h>
#include <Core/EntryPoint.h>
#include "RuntimeLayer.h"

namespace Jah {

	class RuntimeApplication : public Application
	{
	public:
		RuntimeApplication()
		{
			PushLayer(new RuntimeLayer());
		}
	};

	Application* CreateApplication()
	{
		return new RuntimeApplication();
	}

}
