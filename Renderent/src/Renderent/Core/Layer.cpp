#include "repch.h"

#include "Layer.h"

namespace Renderent {

	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName) {}

	Layer::~Layer() {}
}