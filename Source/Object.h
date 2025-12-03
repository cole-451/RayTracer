#pragma once
#include "Color.h"
#include "Ray.h"
class Object
{
public:
	Object() = default;
	Object(const color3_t& color) {
		this->color = color;
	}
	// Making Ray const down the line screws things up in Hit for Sphere.
	virtual bool Hit( Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) = 0;

protected:
	color3_t color;
};