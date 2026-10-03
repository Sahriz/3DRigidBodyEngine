#pragma once

#include <variant>
#include <glm/vec3.hpp>

namespace rbe {

	struct BoxShape {
		glm::vec3 halfExtents{ 0.5f };
	};

	struct SphereShape {
		float radius = 0.5f;
	};

	using Shape = std::variant<BoxShape, SphereShape>;
}