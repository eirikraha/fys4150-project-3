#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include "celestialbody.h"
#include <vector>
#include <string>
#include <fstream>

class SolarSystem
{
public:
    SolarSystem();
    CelestialBody &createCelestialBody(vec3 position, vec3 velocity, double mass);
    void calculateForcesAndEnergy();
    void calculateForcesAndEnergyREL();
    int numberOfBodies() const;

    double totalEnergy() const;
    double potentialEnergy() const;
    double kineticEnergy() const;
    void writeToFile(std::string filename);
    void writeToFilePER(std::string filename, double i, double thetaPrevious);
    vec3 angularMomentum() const;
    vec3 momentum() const;
    std::vector<CelestialBody> &bodies();
    std::vector<CelestialBody> m_bodies;

private:
    vec3 m_angularMomentum;
    vec3 m_momentum;
    std::ofstream m_file;
    double m_kineticEnergy;
    double m_potentialEnergy;
    double pi;
};

#endif // SOLARSYSTEM_H
