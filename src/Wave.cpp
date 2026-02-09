#include "Wave.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

WaveComponent::WaveComponent(double height, double period, double phase, double directionDeg) {
    double g = 9.81;
    
    m_height = height;
    m_period = period;
    m_phase = phase;
    m_direction = directionDeg * M_PI / 180.0;
    m_amplitude = m_height / 2.0;
    m_omega = 2.0 * M_PI / m_period;
    m_wavelength = g * std::pow(m_period,2) / (2.0 * M_PI);
    m_waveNumber = 2.0 * M_PI / m_wavelength;
    m_kx = m_waveNumber * std::cos(m_direction);
    m_kz = m_waveNumber * std::sin(m_direction);
}

double WaveComponent::getElevation(double x, double z, double t) const {
    return m_amplitude * std::cos(m_kx * x + m_kz * z - m_omega * t + m_phase);
}