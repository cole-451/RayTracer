#pragma once
#include <glm/glm.hpp>
#include "Color.h"

struct Ray {

	glm::vec3 origin;
	glm::vec3 direction;


	Ray() = default;

	Ray(const glm::vec3& origin, const glm::vec3& direction) {
		this->origin = origin;
		this->direction = direction;
	}

	// returns a point along the ray.
	glm::vec3 at (const float t) const{
		return origin + t * direction;
	}

	//make it so ray * t returns a point at that distance along the ray.
	glm::vec3 operator *  (const float t) {
		return origin + t * direction;
		
	}

};

struct raycastHit_t {
	glm::vec3 point{ 0 };
	glm::vec3 normal{0};
	float distance;
	class Material* material;
	
};