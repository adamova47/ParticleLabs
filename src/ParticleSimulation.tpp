#include "simulation/ParticleSimulation.h"
#include "utils/Particle.h"


template <typename Storage>
ParticleSimulation<Storage>::ParticleSimulation(const SimulationConfig& config) : config(config), generator(42) {}


template <typename Storage>
void ParticleSimulation<Storage>::initializeParticles() {
    std::uniform_real_distribution<float> positionXGen(config.particleRadius, config.width - config.particleRadius);
    std::uniform_real_distribution<float> positionYGen(config.particleRadius, config.height - config.particleRadius);
    // std::uniform_real_distribution<float> positionZGen(config.particleRadius, config.depth - config.particleRadius);
    std::uniform_real_distribution<float> velocityGen(-50.0f, 50.0f);
    std::uniform_real_distribution<float> radiusGen(1, config.particleRadius);

    storage.resize(config.particleAmount);

    for (int i = 0; i < config.particleAmount; ++i) {

        storage.position(i) = {
            positionXGen(generator),
            positionYGen(generator),
            // positionZGen(generator)
            0.0f
        };

        storage.velocity(i) = {
            velocityGen(generator),
            velocityGen(generator),
            // velocityGen(generator)
            0.0f
        };

        storage.radius(i) = radiusGen(generator);

        storage.mass(i) = 1.0f;
    }
}


template <typename Storage>
void ParticleSimulation<Storage>::setUpSpatialGrids() {
    grid.cellSize = config.particleRadius * 2;

    grid.rows = static_cast<int>(std::ceil(config.height / grid.cellSize));
    grid.columns = static_cast<int>(std::ceil(config.width / grid.cellSize));
    grid.layers = static_cast<int>(std::max(std::ceil(config.depth / grid.cellSize), 1.0f));

    grid.totalCellCount = grid.rows * grid.columns * grid.layers;

    grid.cellCounts.resize(grid.totalCellCount);
    grid.cellOffsets.resize(grid.totalCellCount + 1);
    grid.cellCursor.resize(grid.totalCellCount);

    grid.particleCellIndices.resize(config.particleAmount);
    grid.particleIndices.resize(config.particleAmount);
}


template <typename Storage>
void ParticleSimulation<Storage>::Initialize() {
    initializeParticles();
    setUpSpatialGrids();
}


template <typename Storage>
std::array<int, 3> ParticleSimulation<Storage>::calculateCellCoords(const Vector3 position) const {
    int x = static_cast<int>(std::floor(position.x / grid.cellSize));
    int y = static_cast<int>(std::floor(position.y / grid.cellSize));
    int z = static_cast<int>(std::floor(position.z / grid.cellSize));

    return { x, y, z };
}


template <typename Storage>
int ParticleSimulation<Storage>::calculateCellIndex(const std::array<int, 3>& coords) const {
    const auto [x, y, z] = coords;
    return x + grid.columns * (y + grid.rows * z);
}


template <typename Storage>
void ParticleSimulation<Storage>::buildSpatialGrids() {
    std::fill(grid.cellCounts.begin(), grid.cellCounts.end(), 0);

    for (int i = 0; i < config.particleAmount; i++) {
        int cellIndex = calculateCellIndex(calculateCellCoords(storage.position(i)));

        grid.particleCellIndices[i] = cellIndex;
        grid.cellCounts[cellIndex]++;
    }

    for (int i = 1; i < grid.cellOffsets.size(); i++) {
        grid.cellOffsets[i] = grid.cellOffsets[i - 1] + grid.cellCounts[i - 1];
    }

    grid.cellCursor.assign(grid.cellOffsets.begin(), grid.cellOffsets.end() - 1);

    for (int i = 0; i < config.particleAmount; i++) {
        int cellIndex = grid.particleCellIndices[i];
        int destination = grid.cellCursor[cellIndex];

        grid.particleIndices[destination] = i;
        grid.cellCursor[cellIndex]++;
    }
}


template <typename Storage>
bool ParticleSimulation<Storage>::collisionCheck(
    const Vector3& positionA,
    float radiusA,
    const Vector3& positionB,
    float radiusB
) {
    float dx = positionA.x - positionB.x;
    float dy = positionA.y - positionB.y;
    float dz = positionA.z - positionB.z;

    float radiiSum = radiusA + radiusB;

    return (dx * dx + dy * dy + dz * dz) < (radiiSum * radiiSum);
}


template <typename Storage>
void ParticleSimulation<Storage>::resolveCollision(
    Vector3& positionA,
    Vector3& velocityA,
    float massA,
    Vector3& positionB,
    Vector3& velocityB,
    float massB
) {
    Vector3 normal = {
        positionB.x - positionA.x,
        positionB.y - positionA.y,
        positionB.z - positionA.z
    };
    float distance = sqrt(
        normal.x * normal.x +
        normal.y * normal.y +
        normal.z * normal.z
    );
    normal.x /= distance;
    normal.y /= distance;
    normal.z /= distance;

    Vector3 relativeVelocity = {
        velocityB.x - velocityA.x,
        velocityB.y - velocityA.y,
        velocityB.z - velocityA.z
    };

    float velocityAlongNormal =
        relativeVelocity.x * normal.x +
        relativeVelocity.y * normal.y +
        relativeVelocity.z * normal.z;

    if (velocityAlongNormal < 0) {
        float invMassA = 1.0f / massA;
        float invMassB = 1.0f / massB;

        float invMassSum = invMassA + invMassB;

        float impulse = (-2 * velocityAlongNormal) / invMassSum;

        velocityA.x -= impulse * invMassA * normal.x;
        velocityA.y -= impulse * invMassA * normal.y;
        velocityA.z -= impulse * invMassA * normal.z;

        velocityB.x += impulse * invMassB * normal.x;
        velocityB.y += impulse * invMassB * normal.y;
        velocityB.z += impulse * invMassB * normal.z;
    }

    return;
}


template <typename Storage>
void ParticleSimulation<Storage>::detectCollisions() {
    for (int particleIndex = 0; particleIndex < config.particleAmount; particleIndex++) {
        const auto [x, y, z] = calculateCellCoords(storage.position(particleIndex));
        for (int i = z - 1; i <= z + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                for (int k = x - 1; k <= x + 1; k++) {
                    if (k < 0 || k >= grid.columns ||
                        j < 0 || j >= grid.rows ||
                        i < 0 || i >= grid.layers
                        ) continue;
                    int neighborCell = calculateCellIndex(std::array<int, 3>{k, j, i});
                    for (int l = grid.cellOffsets[neighborCell]; l < grid.cellOffsets[neighborCell + 1]; l++) {
                        int neighborParticleIndex = grid.particleIndices[l];
                        if (particleIndex <= neighborParticleIndex) continue;

                        auto& position = storage.position(particleIndex);
                        auto& otherPosition = storage.position(neighborParticleIndex);
                        float radius = storage.radius(particleIndex);
                        float otherRadius = storage.radius(neighborParticleIndex);

                        bool collision = collisionCheck(position, radius, otherPosition, otherRadius);
                        if (collision) {
                            auto& velocity = storage.velocity(particleIndex);
                            auto& otherVelocity = storage.velocity(neighborParticleIndex);
                            float mass = storage.mass(particleIndex);
                            float otherMass = storage.mass(neighborParticleIndex);
                            resolveCollision(position, velocity, mass, otherPosition, otherVelocity, otherMass);
                        }

                    }
                }
            }
        }
    }
}

template <typename Storage>
void ParticleSimulation<Storage>::moveParticles(float deltaTime) {
    for (int i = 0; i < config.particleAmount; ++i) {
        Vector3& position = storage.position(i);
        const Vector3& velocity = storage.velocity(i);
        float radius = storage.radius(i);

        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        // position.z += velocity.z * deltaTime;

        if (position.x < radius) {
            position.x = radius;
            storage.velocity(i).x *= -1.0f;
        }
        else if (position.x > config.width - radius) {
            position.x = config.width - radius;
            storage.velocity(i).x *= -1.0f;
        }

        if (position.y < radius) {
            position.y = radius;
            storage.velocity(i).y *= -1.0f;
        }
        else if (position.y > config.height - radius) {
            position.y = config.height - radius;
            storage.velocity(i).y *= -1.0f;
        }

        /*
        if (position.z < radius) {
            position.z = radius;
            storage.velocity(i).z *= -1.0f;
        }
        else if (position.z > config.depth - radius) {
            position.z = config.depth - radius;
            storage.velocity(i).z *= -1.0f;
        }
        */
    }
}



template <typename Storage>
void ParticleSimulation<Storage>::Update(float deltaTime) {
    moveParticles(deltaTime);
    buildSpatialGrids();
    detectCollisions();
}


template <typename Storage>
void ParticleSimulation<Storage>::Draw() {
    for (int i = 0; i < config.particleAmount; ++i) {
        const Vector3& position = storage.position(i);
        float radius = storage.radius(i);
        // DrawSphere(position, radius, RED);

        DrawCircle(
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            radius,
            RED
        );
    }
}