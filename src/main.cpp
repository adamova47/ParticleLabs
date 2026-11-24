#include "simulation/ParticleSimulation.h"
#include "simulation/SpatialParticleSimulation.h"
#include "simulation/SimulationConfig.h"
#include "simulation/storage/AoSStorage.h"
#include "simulation/storage/SoAStorage.h"

using AoSParticleSimulation = ParticleSimulation<AoSStorage>;
using SoAParticleSimulation = ParticleSimulation<SoAStorage>;
using AoSSpatialParticleSimulation = SpatialParticleSimulation<AoSStorage>;
using SoASpatialParticleSimulation = SpatialParticleSimulation<SoAStorage>;

#include <raylib.h>


int main()
{
	SimulationConfig config(2500, 1400, 1, 15000, 2.0f);
	InitWindow(config.width, config.height, "Particle Lab");
	SetTargetFPS(60);

	Camera3D camera = { 0 };
	camera.position = { config.width / 2.0f, config.height / 2.0f, -1600.0f };
	camera.target = { config.width / 2.0f, config.height / 2.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 topLeft = { 0, 0, 0 };
	Vector3 topRight = { (float)config.width, 0, 0 };
	Vector3 bottomLeft = { 0, (float)config.height, 0 };
	Vector3 bottomRight = { (float)config.width, (float)config.height, 0 };

	SoAParticleSimulation simulation(config);
	simulation.Initialize();


	while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime(); // Gets the time elapsed since the previous frame

		// UpdateCamera(&camera, CAMERA_ORBITAL);

		simulation.Update(deltaTime);

		BeginDrawing();
		// BeginMode3D(camera);
		ClearBackground(BLACK);

		// Simulation boundaries
		// DrawLine3D(topLeft, topRight, WHITE);
		// DrawLine3D(topRight, bottomRight, WHITE);
		// DrawLine3D(bottomRight, bottomLeft, WHITE);
		// DrawLine3D(bottomLeft, topLeft, WHITE);

		simulation.Draw();
		// EndMode3D();
		EndDrawing();
	}

	CloseWindow();

    return 0;
}
