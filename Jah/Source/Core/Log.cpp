#include "jahpch.h"
#include "Log.h"

namespace Jah {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // optional pattern
		s_CoreLogger = spdlog::stdout_color_mt("JAH");
		s_ClientLogger = spdlog::stdout_color_mt("APP");
	}
	


}