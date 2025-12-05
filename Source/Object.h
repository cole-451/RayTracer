#pragma once
#include "Color.h"
#include "Ray.h"
#include "Transform.h"
#include "Material.h"
#include <memory>
class Object
{
public:
	Object() = default;
	Object(const Transform& transform, std::shared_ptr<Material> material) :
		transform{ transform },
		material{ material }
	{
	}
	virtual bool Hit( const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) = 0;

protected:
	color3_t color;
	Transform transform;
	std::shared_ptr<Material> material;
};