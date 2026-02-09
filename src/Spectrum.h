#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <vector>
#include "Wave.h"

double jonswapSpectrum(double omega, double Hs, double Tp, double gamma = 3.3);
std::vector<WaveComponent> generateJONSWAPWaves(double Hs, double Tp, int numComponents = 50, double directionDeg = 0.0, double gamma = 3.3);

#endif