#include "Renderer.h"
#include "Framebuffer.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Sphere.h"
#include "Random.h"
#include "Material.h"
#include "Plane.h"
#include <algorithm>

int main() {
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;

	// create renderer
	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", SCREEN_WIDTH, SCREEN_HEIGHT);

	Framebuffer framebuffer(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);


	float aspectRatio = framebuffer.width / (float)framebuffer.height;
	Camera camera(20.0f, aspectRatio);
		//camera.SetView({ 0, 0, 5 }, { 0, 0, 0 });
		camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

		Scene scene; // after camera creation/initialization
		// added stuff through final
		//scene.SetSky({ 0,0,0 }, { 1,1,1 });
		auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f, 0.5f, 0.5f));
		scene.AddObject(std::make_unique<Plane>(Transform{ { 0.0f, 0.0f, 0.0f } }, ground_material));

		
		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				glm::vec3 position(a + 0.9f * random::getReal(), 0.2f, b + 0.9f * random::getReal());

				if ((position - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
					std::shared_ptr<Material> sphere_material;

					auto choose_mat = random::getReal();
					if (choose_mat < 0.8f) {
						// diffuse
						auto albedo = HSVtoRGB({ 360.0f * random::getReal(), 1.0f, 1.0f });
						sphere_material = std::make_shared<Lambertian>(albedo);
						scene.AddObject(std::make_unique<Sphere>(Transform{ position }, 0.2f, sphere_material));
					}
					else if (choose_mat < 0.95f) {
						// metal
						auto albedo = color3_t{ random::getReal(0.5f, 1.0f) };
						auto fuzz = random::getReal(0.5f);
						sphere_material = std::make_shared<Metal>(albedo, fuzz);
						scene.AddObject(std::make_unique<Sphere>(Transform{ position }, 0.2f, sphere_material));
					}
					else {
						// glass
						sphere_material = std::make_shared<Dielectric>(HSVtoRGB(360.0f * random::getReal(), 1.0f, 1.0f), 1.0f);
						scene.AddObject(std::make_unique<Sphere>(Transform{ position }, 0.2f, sphere_material));
					}
				}
			}
		}

		auto material1 = std::make_shared<Dielectric>(color3_t{ 1.0f, 1.0f, 1.0f }, 1.5f);
		scene.AddObject(make_unique<Sphere>(Transform{ glm::vec3{ 0.0f, 1.0f, 0.0f } }, 1.0f, material1));

		auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
		scene.AddObject(make_unique<Sphere>(Transform{ glm::vec3{ -4.0f, 1.0f, 0.0f } }, 1.0f, material2));

		auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
		scene.AddObject(make_unique<Sphere>(Transform{ glm::vec3{ 4.0f, 1.0f, 0.0f } }, 1.0f, material3));

		scene.Render(framebuffer, camera, 150);
		//scene.SetSky({1,1,1}, {0,0,1});

		///*auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 1.0f, color3_t{ 0, 1, 0 });
		//auto sphere2 = std::make_unique<Sphere>(glm::vec3{ 1, 1, 2 }, 1.0f, color3_t{ 1, 0, 1 });
		//scene.AddObject(std::move(sphere));
		//scene.AddObject(std::move(sphere2));*/

		//auto red = std::make_shared<Lambertian>(HSVtoRGB(color3_t{ 1.0f, 1.0f, 1.0f }));
		//auto green = std::make_shared<Lambertian>(color3_t{ 0.0f, 1.0f, 0.0f });
		//	auto blue = std::make_shared<Lambertian>(color3_t{ 0.0f, 0.0f, 1.0f });
		//	auto light = std::make_shared<Emissive>(color3_t{ 1.0f, 1.0f, 1.0f }, 3.0f);
		//auto metal = std::make_shared<Metal>(color3_t{ 1.0f, 1.0f, 1.0f }, 0.0f);
		//auto glass = std::make_shared<Dielectric>(color3_t{ 1.0f, 1.0f, 1.0f }, 0.0f);
		//std::shared_ptr<Material> materials[] = { red, green, blue, light, metal, glass };

		//for (int i = 0; i < 25; i++) {
		//	glm::vec3 position = random::getReal(glm::vec3{ -10.0f }, glm::vec3{ 30.0f });
		//	float radius = random::getReal(0.2f, 1.0f);
		//	std::unique_ptr<Object> sphere = std::make_unique<Sphere>(Transform{ position }, radius, materials[random::getInt(5)]);
		//	scene.AddObject(std::move(sphere));
		//}
		//std::unique_ptr<Object> glassSphere = std::make_unique<Sphere>(glm::vec3{20,0,20}, 1.0f, glass);
		//scene.AddObject(std::move(glassSphere));
		//std::unique_ptr<Object> metalSphere = std::make_unique<Sphere>(glm::vec3{-20,10,0}, 1.0f, metal);
		//scene.AddObject(std::move(metalSphere));


		////add plane
		//auto gray = std::make_shared<Lambertian>(color3_t{ 0.2f, 0.2f, 0.2f });
		//std::unique_ptr<Plane> plane = std::make_unique<Plane>(Transform{ glm::vec3{ 0.0f, 0.0f, 0.0f } }, gray);
		//scene.AddObject(std::move(plane));

	// remove previous "static" code and replace with this

	SDL_Event event;
	bool quit = false;
	while (!quit) {
		// check for exit events
		while (SDL_PollEvent(&event)) {
			// window (X) quit
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			// escape key quit
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
			}
		}

		// draw to frame buffer
		//framebuffer.Clear({ 0, 0, 0, 255 });
		//for (int i = 0; i < 300; i++) framebuffer.DrawPoint(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, { 255, 255, 255, 255 });


		// update frame buffer, copy buffer pixels to texture
		framebuffer.Update();

		// copy frame buffer texture to renderer to display
		renderer.CopyFramebuffer(framebuffer);
		renderer.Show();
	}
}