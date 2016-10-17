#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cmath>
#include <time.h>

/* Object-orienting based on example */
#include "vec3.h"
#include "celestialbody.h"
#include "solarsystem.h"
#include "euler.h"

using namespace std;

int main(int argc, char *argv[])
{
    SolarSystem solarSystem; // initializing solar system

    /* Need to do this more properly later, this is just for simple implementation
     * TODO:
     * - Add initial conditions for other planets (PLUTO ISNT A PLANET DAMNIT!) add pluto as well though
     *  - scale mass with the solar mass
     *  - scale distance and velocity to [AU] and [AU/day]? im confused
     *
     * - Add velocity verlet integration and forward euler
     * - Write to file some way / solarsystem has such an elegant way of opening files only once :(
     * - WHAT THE FUCK IS EVEN WRITTEN TO FILE?????
     * - And why does it unexpectedly finish????!?!??!!??!!?!???!?!?!11111
     * - We might have to manually set the Sun position in (0,0,0) each iteration for the first tasks
     *
     * - Maybe add folders in benchmarks for the different tasks, so as to make it more readable?
     *
     * - Need to set up program with some sort of task structure
     *   - 3b: earth-sun system, no solar motion, just write algorithms
     *
     *   - 3c: find initial value giving circular orbit (can be done by math?)
     *     - check stability for Euler's method and velocity Verlet for different dt
     *     - check that both kinetic and potential energy is conserved in circular orbit (+ angular momentum?)
     *     - time both ODE-solvers
     *
     *   - 3d: earth-sun, find escape velocity by trial and error
     *
     *   - 3e: earth-sun-jupiter, sun still in (0,0,0)
     *     - see how much jupiter alters course. change m_j to different values
     *
     *   - 3f: earth-sun-jupiter, sun moves!
     *     - add more planets and moons if time
     *
     *   - 3g: general relativistic correction (mercury!)
     */

    CelestialBody &sun = solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the sun
    // initializing some stuff
    vec3 r_earth(1,0,0);
    vec3 v_earth(0,1,0);
    // Adding earth to celestial body
    solarSystem.createCelestialBody(r_earth, v_earth, 3e-6);

    double dt = 0.001;
    Euler integrator(dt);
    for (int i=0; i<1000; i++)
    {
        integrator.integrateOneStep(solarSystem);
        sun.position = vec3(0,0,0); // might be needed

        solarSystem.writeToFile("../benchmarks/positions.xyz");
    }

    return 0;
}
