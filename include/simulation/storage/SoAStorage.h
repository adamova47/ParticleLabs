#pragma once

#include <raylib.h>
#include <vector>

class SoAStorage {
public:
    std::vector<Vector3> positions;
    std::vector<Vector3> velocities;
    std::vector<float> radii;
    std::vector<float> masses;

    int size() const {
        return static_cast<int>(positions.size());
    }

    void resize(int amount) {
        positions.resize(amount);
        velocities.resize(amount);
        radii.resize(amount);
        masses.resize(amount);
    }

    Vector3& position(int i) {
        return positions[i];
    }

    Vector3& velocity(int i) {
        return velocities[i];
    }

    float& radius(int i) {
        return radii[i];
    }

    float& mass(int i) {
        return masses[i];
    }
};