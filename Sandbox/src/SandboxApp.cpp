#include <Renderent.h>

class ExampleLayer : public Renderent::Layer {
public:
	ExampleLayer() :
		Layer("Example") {}

	void OnUpdate() override {
	}

	void OnEvent(Renderent::Event& event) override {
		RE_TRACE("{0}", event);
	}
};

class Sandbox : public Renderent::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushLayer(new Renderent::ImGuiLayer());
	}

	~Sandbox() {

	}
};



Renderent::Application* Renderent::CreateApplication() {
	return new Sandbox();
}