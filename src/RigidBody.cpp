#include "RigidBody.h"

RigidBody::RigidBody(float mass, float initialY) 
    : m_mass(mass), m_posY(initialY), m_velY(0.0f), m_forceY(0.0f) {}

void RigidBody::resetForces() {
    m_forceY = 0.0f;
}

void RigidBody::addForce(float fy) {
    m_forceY += fy;
}