#include "repch.h"

#include "Renderent/Core/Application.h"
#include "Renderent/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Renderent {


	bool Input::IsKeyPressed(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX() {
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY() {
		auto [x, y] = GetMousePosition();

		return y;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xPos, yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return { xPos, yPos };
	}
}
