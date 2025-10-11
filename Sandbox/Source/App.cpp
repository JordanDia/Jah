#include <Jah.h>
#include <iostream>

class ExampleLayer : public Jah::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		std::cout << "ExampleLayer::Update" << std::endl;
	}

	void OnEvent(Jah::Event& event) override
	{
		std::cout << event.ToString() << std::endl;
	}
};

class App : public Jah::Application
{
public:
	App()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Jah::ImGuiLayer());
	}

	~App()
	{

	}
};


Jah::Application* Jah::CreateApplication()
{
	return new App();
}