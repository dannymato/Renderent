#pragma once



#ifdef RE_PLATFORM_WINDOWS

extern Renderent::Application* Renderent::CreateApplication();

int main(int argc, char** argv) {
	auto app = Renderent::CreateApplication();
	app->Run();
	delete app;
}
#endif