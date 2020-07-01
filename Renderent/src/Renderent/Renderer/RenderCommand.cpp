#include "repch.h"
#include "RenderCommand.h"

#include "Renderent/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Renderent {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}