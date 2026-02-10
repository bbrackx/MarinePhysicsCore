#include "Hydrostatics.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Hydrostatics::Hydrostatics(const HullConfig& hull, float rhoWater, float gravity) 
    : m_rhoWater(rhoWater), m_gravity(gravity) {
    m_waterplaneArea = hull.length * hull.beam;
    m_draft = hull.draft;
} 

void Hydrostatics::applyForces(RigidBody& body) const {
    float mass = body.getMass();
    float gravityForce = -mass * m_gravity;

    float submergedDraft = m_draft - body.getPosY();
    if (submergedDraft < 0.0f) submergedDraft = 0.0f;
    float buoyancyForce = m_rhoWater * m_gravity * m_waterplaneArea * submergedDraft;
    body.addForce(gravityForce + buoyancyForce);
}

float Hydrostatics::getNaturalPeriodHeave(float mass) const {
    float stiffness = m_rhoWater * m_gravity * m_waterplaneArea;
    return 2.0f * M_PI * std::sqrt(mass / stiffness);
}