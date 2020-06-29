#pragma once

#ifdef RE_PLATFORM_WINDOWS

	#ifdef  RE_BUILD_DLL
		#define RENDERENT_API __declspec(dllexport)
	#else
		#define RENDERENT_API __declspec(dllimport)
	#endif
#else
	#error Renderent only supports Windows
#endif 

#ifdef RE_DEBUG
	#define RE_ENABLE_ASSERTS
#endif

#ifdef RE_ENABLE_ASSERTS
	#define RE_ASSERT(x, ...) {if (!(x)) {RE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define RE_CORE_ASSERT(x, ...) {if (!(x)) {RE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define RE_ASSERT(x, ...)
	#define RE_CORE_ASSERT(x, ...)
#endif
#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
