#include "Particle.h"

void Particle::Update(float deltaTime) {
	x += velocityX * deltaTime;
	if (x < 0 + radius) {
		x = 0 + radius;
		velocityX *= -1;
	} else if (x > 1280 - radius) {
		x = 1280 - radius;
		velocityX *= -1;
	}

	y += velocityY * deltaTime;
	if (y < 0 + radius) {
		y = 0 + radius;
		velocityY *= -1;
	}
	else if (y > 800 - radius) {
		y = 800 - radius;
		velocityY *= -1;
	}
}