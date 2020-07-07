#pragma once
#include "Renderent/Log.h"


#ifdef RE_PLATFORM_WINDOWS

extern Renderent::Application* Renderent::CreateApplication();

int main(int argc, char** argv) {
	
	Renderent::Log::init();
	auto app = Renderent::CreateApplication();
	app->Run();
	delete app;
}
#endif
#elif RE_PLATFORM_LINUX

extern Renderent::Application* Renderent::CreateApplication();

int main(int argc, char** argv) {

	Renderent::Log::init();
	auto app = Renderent::CreateApplication();
	app->Run();
	delete app;
}
#endif