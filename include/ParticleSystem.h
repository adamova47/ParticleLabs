#pragma once

#include "Particle.h"
#include <vector>

struct ParticleSystem {

public:
	void Update(float deltaTime);
	void Draw();

private:
	std::vector<Particle> particles{
		Particle{ 100, 100, 0, 50, 50, 0, 1 , 5},
		Particle{ 200, 200, 0, -50, -50, 0, 1 ,5},
		Particle{ 300, 300, 0, 100, -100, 0, 1 , 5}
	}; // dynamic sized collection of particles
};