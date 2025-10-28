#pragma once

#include "Core/Core.h"

extern Jah::Application* Jah::CreateApplication();

int main(int argumentCount, char** arguments)
{
	Jah::Log::Init();

	auto app = Jah::CreateApplication();
	app->Run();
	delete app;
}