#pragma once
#include "Renderent/Core/Log.h"


#ifdef RE_PLATFORM_WINDOWS

extern Renderent::Application* Renderent::CreateApplication();

int main(int argc, char** argv) {
	
	Renderent::Log::init();
	RE_PROFILE_BEGIN_SESSION("Startup", "RenderentStartup-Profile.json");
	auto app = Renderent::CreateApplication();
	RE_PROFILE_END_SESSION();
	RE_PROFILE_BEGIN_SESSION("Running", "RenderentRunning-Profile.json");
	app->Run();
	RE_PROFILE_END_SESSION();
	RE_PROFILE_BEGIN_SESSION("Shutdown", "RenderentShutdown-Profile.json");
	delete app;
	RE_PROFILE_END_SESSION();
}
#elif RE_PLATFORM_LINUX

extern Renderent::Application* Renderent::CreateApplication();

int main(int argc, char** argv) {

	Renderent::Log::init();
	auto app = Renderent::CreateApplication();
	app->Run();
	delete app;
}
#endif