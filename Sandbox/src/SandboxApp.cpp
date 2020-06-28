#include <Renderent.h>

class Sandbox : public Renderent::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Renderent::Application* Renderent::CreateApplication() {
	return new Sandbox();
}