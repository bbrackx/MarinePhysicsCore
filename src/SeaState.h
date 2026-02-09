#ifndef SEASTATE_H
#define SEASTATE_H
#include <vector>
#include "Wave.h"

struct JONSWAPConfig {
    float Hs;
    float Tp;
    float directionDeg;
    float gamma =3.3;
    int numComponents = 50;
};

struct SwellConfig {
    float height;
    float period;
    float directionDeg;
    float phase = 0.0;
};

class SeaState {
private:
std::vector<WaveComponent> m_components;

public:
    void addJONSWAP(const JONSWAPConfig& config);
    void addSwell(const SwellConfig& config);
    float getElevation(float x, float z, float t) const;
    const std::vector<WaveComponent>& getComponents() const { return m_components; }
};

#endif