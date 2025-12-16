#include "jahpch.h"
#include "SoundEngine.h"

namespace Jah {

	void SoundEngine::Init()
	{
		ma_engine_config config = ma_engine_config_init();
		ma_result result = ma_engine_init(&config, &m_Engine);

		JAH_ASSERT(result == MA_SUCCESS, "MiniAudio failed to initialize!");

		ma_engine_set_volume(&m_Engine, 1.0f);
	}

	void SoundEngine::Shutdown()
	{
		ma_engine_uninit(&m_Engine);
	}

	void SoundEngine::PlaySoundFile(const std::filesystem::path& path)
	{
		ma_engine_play_sound(&m_Engine, path.string().c_str(), nullptr);
	}

}