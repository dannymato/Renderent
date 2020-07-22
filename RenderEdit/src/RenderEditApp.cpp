
#include <Renderent.h>
#include <Renderent/Core/Entrypoint.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "EditorLayer.h"

namespace Renderent {

	class RenderEdit : public Application {
	public:
		RenderEdit()
			: RenderEdit(WindowProps()) {

		}

		RenderEdit(const WindowProps& props)
			: Application(props) {

			PushLayer(new EditorLayer());
		}

		~RenderEdit() {

		}
	};


	Application* CreateApplication() {
		return new RenderEdit(WindowProps("RenderEdit", 1920, 1080, true));
	}

}