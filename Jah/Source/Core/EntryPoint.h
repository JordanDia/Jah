#pragma once

extern Jah::Application* Jah::CreateApplication();

int main(int argumentCount, char** arguments)
{
	auto app = Jah::CreateApplication();
	app->Run();
	delete app;
}