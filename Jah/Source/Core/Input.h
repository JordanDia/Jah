#pragma once

#include <utility>

namespace Jah {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		Input() = delete;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
	};


}