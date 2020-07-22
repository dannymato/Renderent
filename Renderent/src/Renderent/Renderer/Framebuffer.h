#pragma once

#include "Renderent/Core/Core.h"

namespace Renderent {

	enum FramebufferAttachments: uint32_t {
		ColorAttachment = BIT(0),
		DepthAttachment = BIT(1)
	};

	struct FramebufferProps {
		uint32_t width = 1280;
		uint32_t height = 720;
		uint32_t samples = 1;
		FramebufferAttachments attachments;
		bool SwapChainTarget = false;
	};

	inline FramebufferAttachments operator|(FramebufferAttachments a, FramebufferAttachments b) {
		return static_cast<FramebufferAttachments>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}

	class Framebuffer {
	public:
		static Ref<Framebuffer> Create(const FramebufferProps& props);

		virtual const FramebufferProps& GetProps() const = 0;

		virtual ~Framebuffer() {}
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Invalidate() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

	};

}