#pragma once

#include <glm/vec3.hpp>

namespace rbe {
	class Body {
	public:
		Body() 
		{
			position	= glm::vec3(0.0f);
			velocity	= glm::vec3(0.0f);
			mass		= 1.0f;
			force		= glm::vec3(0.0f);
		}
		
		Body(glm::vec3 pos, float m) 
		{
			position	= pos;
			velocity	= glm::vec3(0.0f);
			mass		= m;
			force		= glm::vec3(0.0f);
		}
		
		Body(glm::vec3 pos, glm::vec3 v, float m) 
		{
			position	= pos;
			velocity	= v;
			mass		= m;
			force		= glm::vec3(0.0f);
		}


		void AddForce(const glm::vec3 f) 
		{
			force += f;
		}


		glm::vec3 position;
		glm::vec3 velocity;
		float mass;
		glm::vec3 force;
	private:
		
	};
}
