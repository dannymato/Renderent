#pragma once

#include <memory>

#ifdef RE_PLATFORM_WINDOWS
	#ifdef RE_DYNAMIC_LINK
		#ifdef  RE_BUILD_DLL
			#define RENDERENT_API __declspec(dllexport)
		#else
			#define RENDERENT_API __declspec(dllimport)
		#endif
	#else
		#define RENDERENT_API 
	#endif
#elif RE_PLATFORM_LINUX
	#define RENDERENT_API
#else
	#error Renderent only supports Windows
#endif 

#ifdef RE_DEBUG
	#define RE_ENABLE_ASSERTS
#endif

#ifdef RE_PLATFORM_LINUX
	#define DEBUG_BREAK raise(SIGTRAP)
#elif RE_PLATFORM_WINDOWS
	#define DEBUG_BREAK __debugbreak()
#endif

#ifdef RE_ENABLE_ASSERTS
	#define RE_ASSERT(x, ...) {if (!(x)) {RE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK;}}
	#define RE_CORE_ASSERT(x, ...) {if (!(x)) {RE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK;}}
#else
	#define RE_ASSERT(x, ...)
	#define RE_CORE_ASSERT(x, ...)
#endif
#define BIT(x) (1 << x)

#define RE_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


namespace Renderent {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}