#include "velocityverlet.h"
#include "solarsystem.h"
#include<cmath>

VelocityVerlet::VelocityVerlet(double dt) :
    m_dt(dt)
{

}


void VelocityVerlet::integrateOneStep(SolarSystem &system)
{
    system.calculateForcesAndEnergy();

    for(CelestialBody &body : system.bodies()) {
        vec3 force1 = body.force; // force before updating x(i) to x(i+1)
        body.position += body.velocity*m_dt + 0.5 * force1 * pow(m_dt,2);

        // Need to check if "force" = f/m or without div by m
        system.calculateForcesAndEnergy(); // updating forces at x(i+1)
        body.velocity += 0.5* (force1 / body.mass + body.force / body.mass) * m_dt;
    }
}
