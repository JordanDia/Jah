#include <Jah.h>
#include <iostream>
#include <print>

#include <imgui.h>

class ExampleLayer : public Jah::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	
	}

	void OnUpdate() override
	{
			
	}

	void OnEvent(Jah::Event& event) override
	{
	
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::ColorEdit4("", new float[4]);
		ImGui::End();
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