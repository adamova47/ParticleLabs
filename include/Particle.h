#pragma once

struct Particle {
	float x, y, z;
	float velocityX, velocityY, velocityZ;
	float mass;
	int radius;

	void Update(float deltaTime);
};