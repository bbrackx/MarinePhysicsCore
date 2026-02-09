#include <cmath>

#ifndef WAVE_H
#define WAVE_H

class WaveComponent{
private:
    double m_height;
    double m_period;
    double m_amplitude;
    double m_omega;
    double m_wavelength;
    double m_waveNumber;
    double m_phase;
    double m_direction;
    double m_kx;
    double m_kz;

public:
    WaveComponent(double height, double period, double phase = 0.0, double directionDeg = 0.0);

    double getElevation(double x, double z, double t) const;

    double getHeight() const {return m_height;}
    double getPeriod() const {return m_period;}
    double getAmplitude() const {return m_amplitude;}
    double getWavelength() const {return m_wavelength;}
    double getWaveNumber() const {return m_waveNumber;}
    double getOmega() const {return m_omega;}
    double getPhase() const {return m_phase;}
    double getDirectionDeg() const {return m_direction * 180.0 / 3.14159265358979323846;}
};

#endif