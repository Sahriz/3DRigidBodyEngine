#include "RigidBodyEngine/World.hpp"


rbe::BodyID rbe::World::Add(Body* body) {
	BodyID id{ std::size(bodies) };
	bodies.push_back(body);
	return id;
}

void rbe::World::Clear() {
	bodies.clear();
}

void rbe::World::Step(float dt) {
	

	for (size_t i = 0; i < std::size(bodies); i++) {
		Body* body = bodies[i];
		if(body->type == BodyType::STATIC)
			continue;

		if (body->mass == 0.0f) 
			continue;

		body->velocity += dt * (gravity + body->invMass * body->force);
	}

	

	for (size_t i = 0; i < std::size(bodies); i++) {
		Body* body = bodies[i];
		if (body->type == BodyType::STATIC)
			continue;

		body->position += dt * body->velocity;
		body->force = glm::vec3(0.0f);
	}
	
	for (size_t i = 0; i < std::size(bodies); i++) {
		Body* body = bodies[i];
		bool overlaps = false;
		for (size_t j = i + 1; j < std::size(bodies); j++) {
			Body* other = bodies[j];
			if (body->type == BodyType::STATIC && other->type == BodyType::STATIC)
				continue;

			overlaps = body->overlaps(other);
			if (overlaps) {
				// Handle collision response here
				// For simplicity, we will just reverse the velocity of the bodies
				glm::vec3 normal = glm::normalize(other->position - body->position);
				body->velocity = body->type == BodyType::STATIC ? glm::vec3(0.0f) : glm::reflect(body->velocity, glm::normalize(body->velocity));
				other->velocity = other->type == BodyType::STATIC ? glm::vec3(0.0f) : glm::reflect(other->velocity, glm::normalize(other->velocity));
			}
		}
	}

}

rbe::Body* rbe::World::getBody(BodyID id) {
	if (id.index < 0 || id.index >= std::size(bodies)) {
		return nullptr;
	}
	return bodies[id.index];
}
