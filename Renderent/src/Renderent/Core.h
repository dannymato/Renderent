#pragma once

#ifdef RE_PLATFORM_WINDOWS

	#ifdef  RE_BUILD_DLL
		#define RENDERENT_API __declspec(dllexport)
	#else
		#define RENDERENT_API __declspec(dllimport)
	#endif
#else
	#error Renderent only supports Windows
#endif //  
