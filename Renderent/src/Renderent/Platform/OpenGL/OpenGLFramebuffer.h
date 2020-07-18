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

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferProps m_Props;
	};

}