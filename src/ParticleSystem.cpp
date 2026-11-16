#include "ParticleSystem.h"
#include "Particle.h"

#include <raylib.h>

void ParticleSystem::Update(float deltaTime) {
	for (Particle& particle : particles) {
		particle.Update(deltaTime);
	}
}

void ParticleSystem::Draw() {
	for (Particle& particle : particles) {
		DrawCircle(particle.x, particle.y, particle.radius, RED);
	}
}