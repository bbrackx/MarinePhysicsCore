#ifndef RIGIDBODY_H
#define RIGIDBODY_H

class RigidBody {
private:
    float m_mass;

    float m_posY;
    float m_velY;

    float m_forceY;

public:
    RigidBody(float mass, float initialY = 0.0f);
    
    void resetForces();
    void addForce(float fy);

    float getMass() const { return m_mass; }
    float getPosY() const { return m_posY; }
    float getVelY() const { return m_velY; }
    float getForceY() const { return m_forceY; }

    void setPosY(float y) { m_posY = y; }
    void setVelY(float vy) { m_velY = vy; }
};

#endif