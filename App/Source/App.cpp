#include <Core.h>

class App : public Core::Application
{
public:
	App()
	{

	}

	~App()
	{

	}
};


Core::Application* Core::CreateApplication()
{
	return new App();
}