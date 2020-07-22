#pragma once

#include "Renderent/Renderer/Framebuffer.h"

namespace Renderent {

	class OpenGLFramebuffer : public Framebuffer {

	public:
		OpenGLFramebuffer(const FramebufferProps& props);
		virtual ~OpenGLFramebuffer();

		virtual void Invalidate() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const FramebufferProps& GetProps() const override { return m_Props; }

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual void Resize(uint32_t width, uint32_t height);

	private:
		uint32_t m_RendererID = -1;
		uint32_t m_ColorAttachment = -1, m_DepthAttachment = -1;
		FramebufferProps m_Props;
	};

}