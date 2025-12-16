#pragma once

#include "miniaudio.h"
#include <filesystem>

namespace Jah {

	class SoundEngine
	{
	public:
		static void Init();
		static void Shutdown();
		static void PlaySoundFile(const std::filesystem::path& path);

	private:
		inline static ma_engine m_Engine;

	};

}