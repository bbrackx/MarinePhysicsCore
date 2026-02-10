#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "RigidBody.h"
#include <functional>

using ForceFunction = std::function<void(RigidBody& body)>;

class Integrator {
private:
    float m_dt;

public:
    Integrator(float dt);

    void step(RigidBody& body, const ForceFunction& applyAllForces) const;
    float getDT() const { return m_dt;}
};

#endif