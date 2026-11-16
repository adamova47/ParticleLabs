#include "ParticleSystem.h"

#include <raylib.h>

const int windowWidth = 1280;
const int windowHeight = 800;

int main()
{
	InitWindow(windowWidth, windowHeight, "Particle Lab");
	SetTargetFPS(60);

	ParticleSystem simulation;


	while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime(); // Gets the time elapsed since the previous frame
		simulation.Update(deltaTime);
		BeginDrawing();
		ClearBackground(BLACK);
		simulation.Draw();
		EndDrawing();
	}

	CloseWindow();

    return 0;
}
