#include "Integrator.h"

Integrator::Integrator(float dt)
    : m_dt(dt) {
}

void Integrator::step(RigidBody& body, const ForceFunction& applyAllForces) const {
    float mass = body.getMass();
    float y0 = body.getPosY();
    float v0 = body.getVelY();

    // --- RUNGE-KUTTA METHOD ---
    // --- k1: evaluate at current state ---
    body.resetForces();
    applyAllForces(body);
    float a1 = body.getForceY() / mass;
    float dy1 = v0;
    float dv1 = a1;

    // --- k2: evaluate at half step using k1 ---
    body.setPosY(y0 + 0.5f * m_dt * dy1);
    body.setVelY(v0 + 0.5f * m_dt * dv1);
    body.resetForces();
    applyAllForces(body);
    float a2 = body.getForceY() / mass;
    float dy2 = v0 + 0.5f * m_dt * dv1;
    float dv2 = a2;

    // --- k3: evaluate at half step using k2 ---
    body.setPosY(y0 + 0.5f * m_dt * dy2);
    body.setVelY(v0 + 0.5f * m_dt * dv2);
    body.resetForces();
    applyAllForces(body);
    float a3 = body.getForceY() / mass;
    float dy3 = v0 + 0.5f * m_dt * dv2;
    float dv3 = a3;

    // --- k4: evaluate at full step using k3 ---
    body.setPosY(y0 + m_dt * dy3);
    body.setVelY(v0 + m_dt * dv3);
    body.resetForces();
    applyAllForces(body);
    float a4 = body.getForceY() / mass;
    float dy4 = v0 + m_dt * dv3;
    float dv4 = a4;

    // --- Combine: weighted average ---
    float newY = y0 + (m_dt / 6.0f) * (dy1 + 2.0f * dy2 + 2.0f * dy3 + dy4);
    float newV = v0 + (m_dt / 6.0f) * (dv1 + 2.0f * dv2 + 2.0f * dv3 + dv4);

    body.setPosY(newY);
    body.setVelY(newV);
}