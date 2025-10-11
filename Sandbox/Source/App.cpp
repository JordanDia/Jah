#include <Jah.h>
#include <iostream>
#include <print>

class ExampleLayer : public Jah::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//std::cout << "ExampleLayer::Update" << std::endl;

		/*bool holdingA = Jah::Input::IsKeyPressed((int)'A');
		if (holdingA) { std::println("Holding A"); }

		bool mousePressed = Jah::Input::IsMouseButtonPressed(0);
		if (mousePressed) { std::println("Mouse is pressed"); }*/

	}

	void OnEvent(Jah::Event& event) override
	{
		//std::cout << event.ToString() << std::endl;
	}
};

class App : public Jah::Application
{
public:
	App()
	{
		PushLayer(new ExampleLayer());
	}

	~App()
	{

	}
};


Jah::Application* Jah::CreateApplication()
{
	return new App();
}