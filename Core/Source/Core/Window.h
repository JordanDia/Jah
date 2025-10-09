#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "Events/Event.h"


namespace Core {


	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Jah Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		Window(const WindowProps& props = WindowProps());
		~Window();

		void OnUpdate();

		inline uint32_t GetWidth() const { return m_Data.Width; }
		inline uint32_t GetHeight() const { return m_Data.Height; }
		void SetVsync(bool enabled);
		bool IsVSync() const;

		void SetEventCallback(const std::function<void(Event&)>& callback)
		{
			m_Data.EventCallback = callback;
		}

		static Window* Create(const WindowProps& props = WindowProps());


	private:
		void Init(const WindowProps& props);
		void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;
			std::function<void(Event&)> EventCallback;
		};

		WindowData m_Data;
	};

}