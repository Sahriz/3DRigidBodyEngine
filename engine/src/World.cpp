#include "RigidBodyEngine/World.hpp"


rbe::BodyID rbe::World::Add(Body* body) {
	bodies.push_back(body);
	index_amount.index++;
	return index_amount;
}

void rbe::World::Clear() {
	bodies.clear();
}

void rbe::World::Step(float dt) {


	for (size_t i = 0; i < std::size(bodies); i++) {
		Body* body = bodies[i];

		if (body->mass == 0.0f) 
			continue;

		body->velocity += dt * (gravity + body->invMass * body->force);
	}

	for (size_t i = 0; i < std::size(bodies); i++) {
		Body* body = bodies[i];

		body->position += dt * body->velocity;
		body->force = glm::vec3(0.0f);
	}
	std::cout << "Position at: x = " << bodies[0]->position.x << " | y = " << bodies[0]->position.y << " | z = " << bodies[0]->position.z << std::endl;
}
