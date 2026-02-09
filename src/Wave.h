#include <cmath>

#ifndef WAVE_H
#define WAVE_H

class WaveComponent{
private:
    float m_height;
    float m_period;
    float m_amplitude;
    float m_omega;
    float m_wavelength;
    float m_waveNumber;
    float m_phase;
    float m_direction;
    float m_kx;
    float m_kz;

public:
    WaveComponent(float height, float period, float phase = 0.0, float directionDeg = 0.0);

    float getElevation(float x, float z, float t) const;

    float getHeight() const {return m_height;}
    float getPeriod() const {return m_period;}
    float getAmplitude() const {return m_amplitude;}
    float getWavelength() const {return m_wavelength;}
    float getWaveNumber() const {return m_waveNumber;}
    float getOmega() const {return m_omega;}
    float getPhase() const {return m_phase;}
    float getDirectionDeg() const {return m_direction * 180.0 / 3.14159265358979323846;}
};

#endif