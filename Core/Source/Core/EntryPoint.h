#pragma once

extern Core::Application* Core::CreateApplication();

int main(int argumentCount, char** arguments)
{
	auto app = Core::CreateApplication();
	app->Run();
	delete app;
}