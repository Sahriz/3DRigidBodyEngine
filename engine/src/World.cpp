#include "RigidBodyEngine/World.hpp"


void rbe::World::Add(Body* body) {
	bodies.push_back(body);
}

void rbe::World::Clear() {
	bodies.clear();
}

void rbe::World::Step(float dt) {


	for (size_t i = 0; i < std::size(bodies); i++) {
		Body* body = bodies[i];

		if (body->mass == 0.0f) 
			continue;

		body->velocity += dt * (gravity + body->mass * body->force);
	}
}
