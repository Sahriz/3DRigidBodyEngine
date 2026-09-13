#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <iostream>

#include "Body.hpp"
#include "Joint.hpp"
#include "Arbiter.hpp"

namespace rbe {
	class World {
	public:
		World(glm::vec3 grav) : gravity(grav){}

		void Add(Body* body);

		void Clear();

		void Step(float dt);
	private:
		std::vector<Body*> bodies;
		glm::vec3 gravity;
	};
}