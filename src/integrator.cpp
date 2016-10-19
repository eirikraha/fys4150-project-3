#include "integrator.h"
#include "solarsystem.h"
#include <string.h>
#include <cmath>

Integrator::Integrator(double dt, char const *method) :
    m_dt(dt)
{
    num_method = method;
}

void Integrator::integrateOneStep(SolarSystem &system)
{
    system.calculateForcesAndEnergy();

    /* Forward Euler */
    if (strcmp(num_method,"Euler")==0)
    {
        for (CelestialBody &body : system.bodies()) {
            body.position += body.velocity*m_dt;
            body.velocity += body.force / body.mass * m_dt;
        }
    }

    /* Velocity Verlet */
    if (strcmp(num_method,"Verlet")==0)
    {
        for(CelestialBody &body : system.bodies()) {
            vec3 accel1 = body.force/body.mass; // acceleration before updating x(i) to x(i+1)
            body.position += body.velocity*m_dt + 0.5 * accel1 * pow(m_dt,2);

            // Need to check if "force" = f/m or without div by m
            system.calculateForcesAndEnergy(); // updating forces at x(i+1)
            body.velocity += 0.5* (accel1 + body.force / body.mass) * m_dt;
        }
    }
}

