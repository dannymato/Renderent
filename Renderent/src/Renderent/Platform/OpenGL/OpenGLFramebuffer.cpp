#include "repch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Renderent {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferProps& props) {

		m_Props = props;
		Invalidate();

	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
		if (m_Props.attachments & FramebufferAttachments::ColorAttachment)
			glDeleteTextures(1, &m_ColorAttachment);
		if (m_Props.attachments & FramebufferAttachments::DepthAttachment)
			glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate() {

		bool alreadyCreated = m_RendererID != -1;

		if (alreadyCreated)
			glDeleteFramebuffers(1, &m_RendererID);

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		if (m_Props.attachments & FramebufferAttachments::ColorAttachment) {

			if (alreadyCreated)
				glDeleteTextures(1, &m_ColorAttachment);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
			glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Props.width, m_Props.height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}

		if (m_Props.attachments & FramebufferAttachments::DepthAttachment) {

			if (alreadyCreated)
				glDeleteTextures(1, &m_DepthAttachment);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Props.width, m_Props.height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

			RE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is complete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Props.width = width;
		m_Props.height = height;

		Invalidate();
	}

}