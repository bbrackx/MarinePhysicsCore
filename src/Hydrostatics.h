#ifndef HYDROSTATICS_H
#define HYDROSTATICS_H

#include "RigidBody.h"

struct HullConfig{
    float length;
    float beam;
    float depth;
    float draft;
};

class Hydrostatics {
private:
    float m_waterplaneArea;
    float m_draft;
    float m_rhoWater;
    float m_gravity;

public:
    Hydrostatics(const HullConfig& hull, float rhoWater = 1025.0, float gravity = 9.81f);
    void applyForces(RigidBody& body) const;

    float getWaterplaneArea() const { return m_waterplaneArea; }
    float getEquilibriumDraft() const { return m_draft; }
    float getNaturalPeriodHeave(float mass) const;
};

#endif