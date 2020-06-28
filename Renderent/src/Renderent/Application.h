#pragma once
#include "Core.h"


namespace Renderent {


	class RENDERENT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined by client
	Application* CreateApplication();

}

