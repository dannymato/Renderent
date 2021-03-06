#pragma once
#include "repch.h"

#include "Renderent/Core/Core.h"
#include "Renderent/Event/Event.h"

namespace Renderent {

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		WindowProps(const std::string& title = "Renderent Engine",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool vsync = true
			) : Title(title), Width(width), Height(height), VSync(vsync) {}
	};

	class RENDERENT_API Window {

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

		virtual float GetTime() = 0;

	};
}