#pragma once

#include "SimulationConfig.h"

class Simulation {
public:
	virtual ~Simulation() = default;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};