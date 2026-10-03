#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "BodyType.hpp"
#include "Shape.hpp"



namespace rbe {
	class Body {
	public:
		Body()
		{
			type		= BodyType::DYNAMIC;
			position	= glm::vec3(0.0f);
			rotation	= glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			scale		= glm::vec3(1.0f);
			velocity	= glm::vec3(0.0f);
			mass		= 1.0f;
			invMass		= 1.0f / mass;
			force		= glm::vec3(0.0f);
			shape		= BoxShape(glm::vec3(1.0f) * 0.5f);
		}
		Body(BodyType t) 
		{
			type		= t;
			position	= glm::vec3(0.0f);
			rotation	= glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			scale		= glm::vec3(1.0f);
			velocity	= glm::vec3(0.0f);
			mass		= type == BodyType::STATIC ? 0.0f : 1.0f;
			invMass		= type == BodyType::STATIC ? 0.0f : 1.0f / mass;
			force		= glm::vec3(0.0f);
			shape		= BoxShape(glm::vec3(1.0f) * 0.5f);
		}
		
		Body(BodyType t, glm::vec3 pos, float m)
		{
			type		= t;
			position	= pos;
			rotation	= glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			scale		= glm::vec3(1.0f);
			velocity	= glm::vec3(0.0f);
			mass		= type == BodyType::STATIC ? 0.0f : m;
			invMass		= type == BodyType::STATIC ? 0.0f : 1.0f / mass;;
			force		= glm::vec3(0.0f);
			shape		= BoxShape(glm::vec3(1.0f) * 0.5f);
		}
		
		Body(BodyType t, glm::vec3 pos, glm::quat r, glm::vec3 s, float m)
		{
			type		= t;
			position	= pos;
			rotation	= r;
			scale		= s;
			velocity	= glm::vec3(0.0f);;
			mass		= type == BodyType::STATIC ? 0.0f : m;
			invMass		= type == BodyType::STATIC ? 0.0f : 1.0f / mass;;
			force		= glm::vec3(0.0f);
			shape		= BoxShape(s*0.5f);
		}


		void AddForce(const glm::vec3 f) 
		{
			force += f;
		}

		bool overlaps(Body* const other) {
			const rbe::BoxShape* box = std::get_if<rbe::BoxShape>(&shape);
			const rbe::BoxShape* otherBox = std::get_if<rbe::BoxShape>(&other->shape);

			if (box && otherBox) {
				glm::vec3 const& rotatedHalfA = box->halfExtents * rotation;
				glm::vec3 const& rotatedHalfB = otherBox->halfExtents * other->rotation;

				bool overlapX = std::abs(position.x - other->position.x) <= (rotatedHalfA.x + rotatedHalfB.x);
				bool overlapY = std::abs(position.y - other->position.y) <= (rotatedHalfA.y + rotatedHalfB.y);
				bool overlapZ = std::abs(position.z - other->position.z) <= (rotatedHalfA.z + rotatedHalfB.z);

				return	overlapX && overlapY && overlapZ;
			}
			return false;
		}

		BodyType	type;

		Shape		shape;

		glm::vec3	position;
		glm::quat	rotation;
		glm::vec3	scale;
		glm::vec3	velocity;
		float		mass;
		float		invMass;
		glm::vec3	force;
	private:
		
	};
}
