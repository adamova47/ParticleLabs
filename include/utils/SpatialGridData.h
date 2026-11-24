#pragma once

#include <vector>
#include <array>

struct SpatialGridData {
	// dimensions
	int totalCellCount;
	int rows;
	int columns;
	int layers;
	float cellSize;

	// CSR data
	std::vector<int> cellCounts{};
	std::vector<int> cellOffsets{};
	std::vector<int> cellCursor{};
	std::vector<int> particleCellIndices{};
	std::vector<int> particleIndices{};
};