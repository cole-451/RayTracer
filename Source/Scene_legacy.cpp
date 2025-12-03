#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera) {
	// trace ray for every framebuffer pixel
	for (int y = 0; y < framebuffer.height; y++) {
		for (int x = 0; x < framebuffer.width; x++)	{
			// set pixel (x,y) coordinates)
			glm::vec2 pixel{ x,y }; // set pixel x and y
			// normalize (0 <-> 1) the pixel value (pixel / { framebuffer.width, framebuffer.height }
			glm::vec2 point = glm::normalize(pixel / glm::vec2{framebuffer.width, framebuffer.height}); // pixel / { width, height }
			// flip the y value (bottom = 0, top = 1)
			point.y = 1 - point.y;

			// get ray from camera
			Ray ray = camera.GetRay(point); // call GetRay() from camera
				// trace ray
			color3_t color = Trace(ray); // class Trace with ray;
			//color = color3_t{ 1, 0, 0 }; //test

			// draw pixel (x,y) to frame buffer using color (make sure to convert color)
			framebuffer.DrawPoint(pixel.x, pixel.y,  ColorConvert(color));
		}
	}
}

color3_t Scene::Trace(const Ray& ray) {
	// draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(ray.direction); // normalize ray direction
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1) * 0.5f;
	// (direction.y + ??) * ??
	
	// interpolate between sky bottom (0) to sky top (1)
	// mix is the same as lerp, interpolates between values using t (0-1)
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}
