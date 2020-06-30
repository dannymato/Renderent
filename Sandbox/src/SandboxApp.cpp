#include <Renderent.h>

class ExampleLayer : public Renderent::Layer {
public:
	ExampleLayer() :
		Layer("Example") {}

	void OnUpdate() override {
	}

	void OnEvent(Renderent::Event& event) override {
	}

	virtual void OnImGuiRender() override {
	}

};

class Sandbox : public Renderent::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};



Renderent::Application* Renderent::CreateApplication() {
	return new Sandbox();
}