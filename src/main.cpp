//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <fstream>
//#include <cmath>
//#include <time.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <armadillo>
#include <time.h>

/* Object-orienting based on example */
#include "vec3.h"
#include "celestialbody.h"
#include "solarsystem.h"
#include "euler.h"
#include "velocityverlet.h"
#include "integrator.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 5){
        cout<<"Usage: "<<argv[0]<<" Task"<<" dt"<<" task_param"<<" Method"<<endl;
        cout<<"Task and dt always required, task_param and Method optional and depends on Task."<<"\n\n";
        cout<<"   Task : ES, ESesc,ESJ, WS or GM"<<endl;
        cout<<"   ES : Earth-Sun system. The Sun is fixed to origo."<<endl;
        cout<<"   ESesc: Earth-Sun system, find escape velocity. The Sun is fixed in origo." << endl;
        cout<<"   ESJ : Earth-Sun-Jupiter system. The Sun is fixed to origo."<<endl;
        cout<<"   WS : Whole solar system, including Pluto. The Sun is not fixed to origo."<<endl;
        cout<<"   GM : General relativity with Mercury (not Freddie)."<<endl; //change this when we have understood the task.
        cout<<"   dt : time step" << endl;
        cout<<"   task_param : Meaning depends on Task"<<endl;
        cout<<"     -ES/ESesc : task_param is initial velocity of the Earth in y direction [AU/yr]."<<endl;
        cout<<"     -ESJ: task_param spesifies a number multiplied with mass of Jupiter to alter it."<<endl;
        cout<<"   Method : Euler or Verlet, set to Verlet if not specified" << endl;
        cout<<"    Note that Verlet is actually velocity Verlet, but the name is fairly long, hence the shortening."<<endl;
        exit(1);
    }

    /* Specifying the algorithm used to solve numerically */
    char const *method = "Verlet";
    if (argc == 5)
    {
        if (!(strcmp(argv[4],"Euler")==0 || strcmp(argv[4],"Verlet")==0))
        {
            cout<<"Method not specified correctly, use 'Euler' or  'Verlet'. Set to Verlet by default."<<endl;
        }
        else
        {
            method = argv[4];
        }
    }

    /*
    *if (argc < 5)
    *{
    *    char method = "Verlet";
    *}
    *else if (argc == 5)
    *{
    *    if(strcmp(argv[4], "Euler") == 0)  //Checks which method is specified. Make it more fancy another day.
    *    {
    *        int method = 1;
    *    }
    *    else
    *    {
    *        int method = 0;
    *    }
    *}
    */
    SolarSystem solarSystem; // initializing solar system
    double dt = atof(argv[2]);
    int num_timesteps = 1000;

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

    /////////////////////////////////////////////
    ///    Earth-Sun system, Sun is fixed     ///
    /////////////////////////////////////////////

    if (strcmp(argv[1], "ES") == 0)
    {
        solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the sun
        // initializing some stuff

        double v_y0 = 6.28; // not completely random default value
        if (argc > 3)
        {
            v_y0 = atof(argv[3]);
        }

        vec3 r_earth(1,0,0);
        vec3 v_earth(0,v_y0,0);
        // Adding earth to celestial body

        solarSystem.createCelestialBody(r_earth, v_earth, 3e-6);

        solarSystem.writeToFile("../benchmarks/positions.xyz", 0); //initializing file
        Integrator integrator(dt,method);
        // Euler integrator(dt);
        // VelocityVerlet integrator(dt);

        //Access sun position so that we can keep it still
        std::vector<CelestialBody> &m_bodies = solarSystem.bodies();
        CelestialBody &body = m_bodies[0];

        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);
            body.position = vec3(0,0,0); // fixing the Sun to (0,0,0) each step
            solarSystem.writeToFile("../benchmarks/positions.xyz", i);
        }
    }

    else if(strcmp(argv[1], "ESesc") == 0)
    {
        cout << "Now these points of data make a beautiful line." << endl;
    }
    else if(strcmp(argv[1], "ESJ") == 0)
    {
        cout << "And we're out of beta, we're releasing on time." << endl;
    }
    else if(strcmp(argv[1], "WS") == 0)
    {
        cout << "So I'm GLaD I got burned." << endl;
    }
    else if(strcmp(argv[1], "GM") == 0)
    {
        cout << "Think of all the things we learned." << endl;
    }
    else
    {
        cout << "For the people who are still alive." << endl;
    }


    return 0;
}
