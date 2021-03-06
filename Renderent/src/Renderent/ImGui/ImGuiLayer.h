#pragma once

#include "Renderent/Core/Core.h"
#include "Renderent/Core/Layer.h"
#include "Renderent/Event/KeyEvent.h"
#include "Renderent/Event/MouseEvent.h"
#include "Renderent/Event/WindowEvent.h"

namespace Renderent {

	class RENDERENT_API ImGuiLayer : public Layer {

	public: 
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0;

	};

}