#ifndef SEASTATE_H
#define SEASTATE_H
#include <vector>
#include "Wave.h"

struct JONSWAPConfig {
    double Hs;
    double Tp;
    double directionDeg;
    double gamma =3.3;
    int numComponents = 50;
};

struct SwellConfig {
    double height;
    double period;
    double directionDeg;
    double phase = 0.0;
};

class SeaState {
private:
std::vector<WaveComponent> m_components;

public:
    void addJONSWAP(const JONSWAPConfig& config);
    void addSwell(const SwellConfig& config);
    double getElevation(double x, double z, double t) const;
    const std::vector<WaveComponent>& getComponents() const { return m_components; }
};

#endif