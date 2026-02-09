#include "Spectrum.h"
#include <cmath>
#include <vector>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float jonswapSpectrum(float omega, float Hs, float Tp, float gamma){
    if (omega <= 0.0) return 0.0;

    float omegaP = 2.0 * M_PI / Tp;
    float spm = (5.0 / 16.0) * std::pow(Hs, 2.0) * std::pow(omegaP, 4.0) * std::pow(omega, -5.0) * std::exp(-1.25 * std::pow(omega / omegaP, -4.0));
    
    float sigma = (omega <= omegaP) ? 0.07 : 0.09;
    float b = std::exp(-0.5 * std::pow((omega - omegaP) / (sigma * omegaP), 2.0));
    float aGamma = 1.0 - 0.287 * std::log(gamma);

    float S = aGamma * spm * std::pow(gamma,b);

    return S;
}

std::vector<WaveComponent> generateJONSWAPWaves(float Hs, float Tp, int numComponents, float directionDeg, float gamma) {
    std::vector<WaveComponent> waves;

    float omegaP = 2.0 * M_PI / Tp;
    float omegaMin = 0.3 * omegaP;
    float omegaMax = 3.0 * omegaP;
    float deltaOmega = (omegaMax - omegaMin) / numComponents;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> phaseDist(0.0, 2.0 * M_PI);

    for (int i = 0; i < numComponents; i++){
        float omega = omegaMin + (i + 0.5) * deltaOmega;
        float S_omega = jonswapSpectrum(omega, Hs, Tp);
        float amplitude = std::sqrt(2.0 * S_omega * deltaOmega);
        float height = 2.0 * amplitude;
        float period = 2.0 * M_PI / omega;
        float phase = phaseDist(gen);
        
        waves.push_back(WaveComponent(height, period, phase, directionDeg));
    }

    return waves;
}