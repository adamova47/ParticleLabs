#pragma once

#include "Simulation.h"
#include "SimulationConfig.h"
#include "utils/Particle.h"
#include "utils/SpatialGridData.h"

#include <array>
#include <raylib.h>
#include <random>


template <typename Storage>
class ParticleSimulation : public Simulation {
protected:
    SimulationConfig config;
    std::mt19937 generator;
    Storage storage;
    SpatialGridData grid;

public:
    explicit ParticleSimulation(const SimulationConfig& config);

    void initializeParticles();
    virtual void setUpSpatialGrids(); // for virtual to work all the corresponding base-class methods have to be virtual
    // virtual tells C++ that a derived class is allowed to provide a replacement implementation, 
    // and that the replacement should be selected when calling through a base-class reference or pointer.

    void Initialize() override;

    std::array<int, 3> calculateCellCoords(const Vector3 position) const;
    int calculateCellIndex(const std::array<int, 3>& coords) const;

    virtual void buildSpatialGrids();

    bool collisionCheck(const Vector3& positionA, float radiusA, const Vector3& positionB, float radiusB);
    void resolveCollision(Vector3& positionA, Vector3& velocityA, float massA, Vector3& positionB, Vector3& velocityB, float massB);
    virtual void detectCollisions();

    void moveParticles(float deltaTime);

    void Update(float deltaTime) override;
    void Draw() override;
};


#include "ParticleSimulation.tpp"