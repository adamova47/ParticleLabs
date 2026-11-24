#pragma once

#include <random>

struct SimulationConfig
{
    int width;
    int height;
    int depth;
    int particleAmount;
    float particleRadius;

    SimulationConfig(int width, int height, int depth, int particleAmount, float particleRadius)
        : 
        width(width),
        height(height),
        depth(depth),
        particleAmount(particleAmount),
        particleRadius(particleRadius)
    {}
};