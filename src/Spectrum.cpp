#include "Spectrum.h"
#include <cmath>
#include <vector>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double jonswapSpectrum(double omega, double Hs, double Tp, double gamma){
    if (omega <= 0.0) return 0.0;

    double omegaP = 2.0 * M_PI / Tp;
    double spm = (5.0 / 16.0) * std::pow(Hs, 2.0) * std::pow(omegaP, 4.0) * std::pow(omega, -5.0) * std::exp(-1.25 * std::pow(omega / omegaP, -4.0));
    
    double sigma = (omega <= omegaP) ? 0.07 : 0.09;
    double b = std::exp(-0.5 * std::pow((omega - omegaP) / (sigma * omegaP), 2.0));
    double aGamma = 1.0 - 0.287 * std::log(gamma);

    double S = aGamma * spm * std::pow(gamma,b);

    return S;
}

std::vector<WaveComponent> generateJONSWAPWaves(double Hs, double Tp, int numComponents, double directionDeg, double gamma) {
    std::vector<WaveComponent> waves;

    double omegaP = 2.0 * M_PI / Tp;
    double omegaMin = 0.3 * omegaP;
    double omegaMax = 3.0 * omegaP;
    double deltaOmega = (omegaMax - omegaMin) / numComponents;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> phaseDist(0.0, 2.0 * M_PI);

    for (int i = 0; i < numComponents; i++){
        double omega = omegaMin + (i + 0.5) * deltaOmega;
        double S_omega = jonswapSpectrum(omega, Hs, Tp);
        double amplitude = std::sqrt(2.0 * S_omega * deltaOmega);
        double height = 2.0 * amplitude;
        double period = 2.0 * M_PI / omega;
        double phase = phaseDist(gen);
        
        waves.push_back(WaveComponent(height, period, phase, directionDeg));
    }

    return waves;
}