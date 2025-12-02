#pragma once
#include <glm/glm.hpp>

struct Ray {

	glm::vec3 origin;
	glm::vec3 direction;


	Ray() = default;

	Ray(const glm::vec3& origin, const glm::vec3& direction) {
		this->origin = origin;
		this->direction = direction;
	}

	// returns a point along the ray.
	glm::vec3 at(const float t) {
		return origin + t * direction;
	}

	//make it so ray * t returns a point at that distance along the ray.
	glm::vec3 operator *  (const float t) {
		return origin + t * direction;
		
	}

};