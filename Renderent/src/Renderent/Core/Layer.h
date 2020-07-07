#pragma once

#include "Renderent/Core/Core.h"
#include "Renderent/Core/TimeStep.h"
#include "Renderent/Event/Event.h"

namespace Renderent {

	class RENDERENT_API Layer {

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep timestep) {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& e) {};

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;

	};

}