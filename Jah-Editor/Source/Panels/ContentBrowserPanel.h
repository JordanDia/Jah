#pragma once

#include "Renderer/Texture.h"
#include <filesystem>

namespace Jah {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;

		Shared<Texture2D> m_DirectoryIcon;
		Shared<Texture2D> m_FileIcon;
	};

}