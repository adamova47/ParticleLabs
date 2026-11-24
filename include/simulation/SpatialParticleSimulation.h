#pragma once

#include "ParticleSimulation.h"


template <typename Storage>
class SpatialParticleSimulation : public ParticleSimulation<Storage> {
protected:
	Storage sortedStorage;
public:
	using Base = ParticleSimulation<Storage>;
	explicit SpatialParticleSimulation(const SimulationConfig& config) : Base(config) {}

	void setUpSpatialGrids() override {
		Base::setUpSpatialGrids(); // Base is important here becayse were choosing the parent implementation 
		sortedStorage.resize(this->config.particleAmount);
	}

	void buildSpatialGrids() override {
		std::fill(this->grid.cellCounts.begin(), this->grid.cellCounts.end(), 0);

		for (int i = 0; i < this->config.particleAmount; i++) {
			int cellIndex = this->calculateCellIndex(
				this->calculateCellCoords(
					this->storage.position(i)
				)
			);

			this->grid.particleCellIndices[i] = cellIndex;
			this->grid.cellCounts[cellIndex]++;
		}

		for (int i = 1; i < this->grid.cellOffsets.size(); i++) {
			this->grid.cellOffsets[i] = this->grid.cellOffsets[i - 1] + this->grid.cellCounts[i - 1];
		}

		this->grid.cellCursor.assign(
			this->grid.cellOffsets.begin(), 
			this->grid.cellOffsets.end() - 1
		);

		for (int i = 0; i < this->config.particleAmount; i++) {
			int cellIndex = this->grid.particleCellIndices[i];
			int destination = this->grid.cellCursor[cellIndex];

			sortedStorage.position(destination) = this->storage.position(i);
			sortedStorage.velocity(destination) = this->storage.velocity(i);
			sortedStorage.radius(destination) = this->storage.radius(i);
			sortedStorage.mass(destination) = this->storage.mass(i);
			
			this->grid.cellCursor[cellIndex]++;
		}

		std::swap(this->storage, sortedStorage);
	}

	void detectCollisions() override {
		for (int particleIndex = 0; particleIndex < this->config.particleAmount; particleIndex++) {
			
			const auto [x, y, z] = 
				this->calculateCellCoords(
					this->storage.position(particleIndex)
				);
			
			for (int i = z - 1; i <= z + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {
					for (int k = x - 1; k <= x + 1; k++) {
						if (k < 0 || k >= this->grid.columns ||
							j < 0 || j >= this->grid.rows ||
							i < 0 || i >= this->grid.layers
							) continue;

						int neighborCell = this->calculateCellIndex(std::array<int, 3>{k, j, i});
						
						for (int l = this->grid.cellOffsets[neighborCell]; l < this->grid.cellOffsets[neighborCell + 1]; l++) {
							if (particleIndex <= l) continue;

							auto& position = this->storage.position(particleIndex);
							auto& otherPosition = this->storage.position(l);
							float radius = this->storage.radius(particleIndex);
							float otherRadius = this->storage.radius(l);

							bool collision = this->collisionCheck(position, radius, otherPosition, otherRadius);
							if (collision) {
								auto& velocity = this->storage.velocity(particleIndex);
								auto& otherVelocity = this->storage.velocity(l);
								float mass = this->storage.mass(particleIndex);
								float otherMass = this->storage.mass(l);
								this->resolveCollision(position, velocity, mass, otherPosition, otherVelocity, otherMass);
							}

						}
					}
				}
			}
		}
	}

	void Update(float deltaTime) override {
		this->moveParticles(deltaTime);
		buildSpatialGrids();
		detectCollisions();
	}
};