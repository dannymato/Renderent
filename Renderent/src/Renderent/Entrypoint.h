#pragma once



#ifdef RE_PLATFORM_WINDOWS

extern Renderent::Application* Renderent::CreateApplication();

int main(int argc, char** argv) {

	Renderent::Log::init();
	RE_CORE_WARN("Initialized Log!");
	int a = 5;
	RE_INFO("Hello! Var={0}", a);

	auto app = Renderent::CreateApplication();
	app->Run();
	delete app;
}
#endif