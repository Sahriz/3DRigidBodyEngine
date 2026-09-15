#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "BodyType.hpp"



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
		}


		void AddForce(const glm::vec3 f) 
		{
			force += f;
		}

		BodyType type;

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 velocity;
		float mass;
		float invMass;
		glm::vec3 force;
	private:
		
	};
}
