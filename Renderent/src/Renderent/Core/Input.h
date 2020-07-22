#pragma once

#include "Renderent/Core/Core.h"

namespace Renderent {

	class RENDERENT_API Input {
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

	};

}