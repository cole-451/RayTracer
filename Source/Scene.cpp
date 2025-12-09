#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Random.h"
#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples) {
	// trace ray for every framebuffer pixel
	for (int y = 0; y < framebuffer.height; y++) {
		for (int x = 0; x < framebuffer.width; x++) {
			// color will be accumulated with ray trace samples
			color3_t color{ 0 };
			// multi-sample for each pixel
			for (int i = 0; i < numSamples; i++) {
				// set pixel (x,y) coordinates)
				glm::vec2 pixel{ x, y };
				// add random value (0-1) to pixel valie, each sample should be a little different
				pixel += glm::vec2{random::getReal(1.0f), random::getReal(1.0f)};
				// normalize (0 <-> 1) the pixel value (pixel / vec2{ framebuffer.width, framebuffer.height }
				glm::vec2 point = pixel / glm::vec2{ framebuffer.width, framebuffer.height };
				// flip the y value (bottom = 0, top = 1)
				point.y = 1 - point.y;

				// get ray from camera
				Ray ray = camera.GetRay(point);
				// trace ray, still needs a raycast hit
				color += Trace(ray, 0.0001f, 100.0f, 20);
			}
			// get average color = (color / number samples)
			color = color3_t{color / color3_t{(float)numSamples}};
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}

void Scene::AddObject(std::unique_ptr<Object> object) {
	// add object to objects vector
	//because it is a unique pointer, you need to transfer ownership, or else it will throw an exception.
	objects.push_back(std::move(object));
}

color3_t Scene::Trace(const Ray& ray, float minDistance, float maxDistance, int maxDepth) {

	// reached max depth (bounce) return black color
	if (maxDepth == 0) return { 0,0,0 };

		
	raycastHit_t raycastHit;
	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (auto& object : objects) {
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))	{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance; // raycast hit distance;
		}
	}

	// check if ray hit object
	if (rayHit)	{
		color3_t attenuation;
		Ray scattered;
		// get raycast hit matereial, get material color and scattered ray 
		if (raycastHit.material->Scatter(ray, raycastHit, attenuation, scattered)) {
			// trace scattered ray, final color will be the product of all the material colors
			return attenuation * Trace(scattered, minDistance, maxDistance, --maxDepth);
		}
		else {
			return raycastHit.material->GetEmissive();
		}
	}

	// draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(ray.direction);
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1) * 0.5f;
	
	// interpolate between sky bottom (0) to sky top (1)
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}
