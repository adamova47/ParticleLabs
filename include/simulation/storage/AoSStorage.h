#pragma once

#include "utils/Particle.h"

#include <vector>

class AoSStorage {
public:
    std::vector<Particle> particles;

    int size() const {
        return static_cast<int>(particles.size());
    }

    void resize(int amount) {
        particles.resize(amount);
    }

    Vector3& position(int i) {
        return particles[i].position;
    }

    Vector3& velocity(int i) {
        return particles[i].velocity;
    }

    float& radius(int i) {
        return particles[i].radius;
    }

    float& mass(int i) {
        return particles[i].mass;
    }
};