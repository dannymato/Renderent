#pragma once

#include "Renderent/Core/Core.h"

namespace Renderent {

	struct FramebufferProps {
		uint32_t width = 1280;
		uint32_t height = 720;
		uint32_t samples = 1;

		bool SwapChainTarget = false;


	};

	class Framebuffer {
	public:
		static Ref<Framebuffer> Create(const FramebufferProps& props);

		virtual const FramebufferProps& GetProps() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Invalidate() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
	};

}