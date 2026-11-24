#pragma once

#include "raylib.h"

struct Particle {
	Vector3 position;
	Vector3 velocity;
	float mass;
	float radius;

	void Update(float deltaTime);
};