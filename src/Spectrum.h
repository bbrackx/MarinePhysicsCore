#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <vector>
#include "Wave.h"

float jonswapSpectrum(float omega, float Hs, float Tp, float gamma = 3.3);
std::vector<WaveComponent> generateJONSWAPWaves(float Hs, float Tp, int numComponents = 50, float directionDeg = 0.0, float gamma = 3.3);

#endif