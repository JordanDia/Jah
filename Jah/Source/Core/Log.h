#pragma once

#include "Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/fmt.h>
#pragma warning(pop)

namespace Jah {

	class Log
	{
	public:
		static void Init();

		inline static Shared<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Shared<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Shared<spdlog::logger> s_CoreLogger;
		static Shared<spdlog::logger> s_ClientLogger;
	};

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

#define JAH_CORE_TRACE(...)			::Jah::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define JAH_CORE_INFO(...)			::Jah::Log::GetCoreLogger()->info(__VA_ARGS__)
#define JAH_CORE_WARN(...)			::Jah::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define JAH_CORE_ERROR(...)			::Jah::Log::GetCoreLogger()->error(__VA_ARGS__)
#define JAH_CORE_CRITICAL(...)		::Jah::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define JAH_TRACE(...)				::Jah::Log::GetClientLogger()->trace(__VA_ARGS__)
#define JAH_INFO(...)				::Jah::Log::GetClientLogger()->info(__VA_ARGS__)
#define JAH_ERROR(...)				::Jah::Log::GetClientLogger()->error(__VA_ARGS__)
#define JAH_WARN(...)				::Jah::Log::GetClientLogger()->warn(__VA_ARGS__)
#define JAH_CRITICAL(...)			::Jah::Log::GetClientLogger()->critical(__VA_ARGS__)