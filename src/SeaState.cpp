#include "SeaState.h"
#include "Spectrum.h"

void SeaState::addJONSWAP(const JONSWAPConfig& config) {
    auto waves = generateJONSWAPWaves(config.Hs, config.Tp, config.numComponents, config.directionDeg, config.gamma);
    m_components.insert(m_components.end(), waves.begin(), waves.end());
}

void SeaState::addSwell(const SwellConfig& config) {
    m_components.push_back(WaveComponent(config.height, config.period, config.phase, config.directionDeg));
}

float SeaState::getElevation(float x, float z, float t) const {
    float eta = 0.0;
    for (const auto& w : m_components) {
        eta += w.getElevation(x, z, t);
    }
    return eta;
}