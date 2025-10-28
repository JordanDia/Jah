#pragma once

#include <Jah.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Jah {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define JAH_CORE_ERROR(...)		::Jah::Log::GetCoreLogger()->error(__VA_ARGS__)
#define JAH_CORE_WARN(...)		::Jah::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define JAH_CORE_INFO(...)		::Jah::Log::GetCoreLogger()->info(__VA_ARGS__)
#define JAH_CORE_TRACE(...)		::Jah::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define JAH_CORE_FATAL(...)		::Jah::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define JAH_ERROR(...)		::Jah::Log::GetCoreLogger()->error(__VA_ARGS__)
#define JAH_WARN(...)		::Jah::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define JAH_INFO(...)		::Jah::Log::GetCoreLogger()->info(__VA_ARGS__)
#define JAH_TRACE(...)		::Jah::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define JAH_FATAL(...)		::Jah::Log::GetCoreLogger()->fatal(__VA_ARGS__)