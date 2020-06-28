#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

#include <Memory>

namespace Renderent {
	class RENDERENT_API Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};


}

#define RE_CORE_FATAL(...) ::Renderent::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define RE_CORE_ERROR(...) ::Renderent::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_WARN(...) ::Renderent::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_INFO(...) ::Renderent::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_TRACE(...) ::Renderent::Log::GetCoreLogger()->trace(__VA_ARGS__)


#define RE_FATAL(...) ::Renderent::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define RE_ERROR(...) ::Renderent::Log::GetClientLogger()->error(__VA_ARGS__)
#define RE_WARN(...)  ::Renderent::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RE_INFO(...)  ::Renderent::Log::GetClientLogger()->info(__VA_ARGS__)
#define RE_TRACE(...) ::Renderent::Log::GetClientLogger()->trace(__VA_ARGS__)