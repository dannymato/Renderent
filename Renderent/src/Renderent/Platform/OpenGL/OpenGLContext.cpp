#include "repch.h"

#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Renderent {

	OpenGLContext::OpenGLContext(GLFWwindow* window) : m_WindowHandle(window) {

		RE_CORE_ASSERT(window, "Window Handle is null");

	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RE_CORE_ASSERT(status, "Failed to initialize Glad!");

		RE_CORE_INFO("OpenGL Renderer: ");
		RE_CORE_INFO("Vendor:\t{0}", glGetString(GL_VENDOR));
		RE_CORE_INFO("Renderer:\t{0}", glGetString(GL_RENDERER));
		RE_CORE_INFO("Version:\t{0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		
		glfwSwapBuffers(m_WindowHandle);
	}

}