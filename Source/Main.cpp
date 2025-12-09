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
	Camera camera(70.0f, aspectRatio);
		//camera.SetView({ 0, 0, 5 }, { 0, 0, 0 });
		camera.SetView({ 0, 5, 30 }, { 0, 0, 0 });

		Scene scene; // after camera creation/initialization
		scene.SetSky({1,1,1}, {0,0,1});

		/*auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 1.0f, color3_t{ 0, 1, 0 });
		auto sphere2 = std::make_unique<Sphere>(glm::vec3{ 1, 1, 2 }, 1.0f, color3_t{ 1, 0, 1 });
		scene.AddObject(std::move(sphere));
		scene.AddObject(std::move(sphere2));*/

		auto red = std::make_shared<Lambertian>(HSVtoRGB(color3_t{ 1.0f, 1.0f, 1.0f }));
		auto green = std::make_shared<Lambertian>(color3_t{ 0.0f, 1.0f, 0.0f });
			auto blue = std::make_shared<Lambertian>(color3_t{ 0.0f, 0.0f, 1.0f });
			auto light = std::make_shared<Emissive>(color3_t{ 1.0f, 1.0f, 1.0f }, 3.0f);
		auto metal = std::make_shared<Metal>(color3_t{ 1.0f, 1.0f, 1.0f }, 0.0f);
		auto glass = std::make_shared<Dielectric>(color3_t{ 1.0f, 1.0f, 1.0f }, 0.0f);
		std::shared_ptr<Material> materials[] = { red, green, blue, light, metal, glass };

		for (int i = 0; i < 25; i++) {
			glm::vec3 position = random::getReal(glm::vec3{ -10.0f }, glm::vec3{ 30.0f });
			float radius = random::getReal(0.2f, 1.0f);
			std::unique_ptr<Object> sphere = std::make_unique<Sphere>(Transform{ position }, radius, materials[random::getInt(5)]);
			scene.AddObject(std::move(sphere));
		}
		std::unique_ptr<Object> glassSphere = std::make_unique<Sphere>(glm::vec3{20,0,20}, 1.0f, glass);
		scene.AddObject(std::move(glassSphere));
		std::unique_ptr<Object> metalSphere = std::make_unique<Sphere>(glm::vec3{-20,10,0}, 1.0f, metal);
		scene.AddObject(std::move(metalSphere));


		//add plane
		auto gray = std::make_shared<Lambertian>(color3_t{ 0.2f, 0.2f, 0.2f });
		std::unique_ptr<Plane> plane = std::make_unique<Plane>(Transform{ glm::vec3{ 0.0f, 0.0f, 0.0f } }, gray);
		scene.AddObject(std::move(plane));

		

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
		framebuffer.Clear({ 0, 0, 0, 255 });
		//for (int i = 0; i < 300; i++) framebuffer.DrawPoint(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, { 255, 255, 255, 255 });

		// remove previous "static" code and replace with this
		scene.Render(framebuffer, camera, 100);
		//scene.SetSky({ 1,1,0 }, { 1,0,0 });

		// update frame buffer, copy buffer pixels to texture
		framebuffer.Update();

		// copy frame buffer texture to renderer to display
		renderer.CopyFramebuffer(framebuffer);
		renderer.Show();
	}
}