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
        if (!(strcmp(argv[4],"Euler")==0 || strcmp(argv[4],"Verlet")==0 || strcmp(argv[4], "VerletREL") == 0))
        {
            cout<<"Method not specified correctly, use 'Euler', 'Verlet' or 'VerletREL'. Set to Verlet by default."<<endl;
        }
        else
        {
            method = argv[4];
        }
    }

    SolarSystem solarSystem; // initializing solar system
    double dt = atof(argv[2]);
    int num_timesteps = 1000;

    /* Need to do this more properly later, this is just for simple implementation
     * TODO:
     * - Add initial conditions for other planets (PLUTO ISNT A PLANET DAMNIT!) add pluto as well though
     *  - scale mass with the solar mass
     *  - scale distance and velocity to [AU] and [AU/day]? im confused
     *
     * - Write to file some way / solarsystem has such an elegant way of opening files only once :(
     * - WHAT THE FUCK IS EVEN WRITTEN TO FILE?????
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
        double v_y0 = 6.28; // not completely random default value
        if (argc > 3)
        {
            v_y0 = atof(argv[3]);
        }

        // Creating filename for this task
        string filename = "../benchmarks/ES/pos_dt"+to_string(dt)+"_N"+to_string(num_timesteps)+"_vy"+to_string(v_y0)+".xyz";

        solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the sun
        // initializing some stuff

        vec3 r_earth(1,0,0);
        vec3 v_earth(0,v_y0,0);
        // Adding earth to celestial body

        solarSystem.createCelestialBody(r_earth, v_earth, 3e-6);

        solarSystem.writeToFile(filename); //initializing file
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
            solarSystem.writeToFile(filename);
        }
    }

    else if(strcmp(argv[1], "ESesc") == 0)
    {
        cout << "Now these points of data make a beautiful line." << endl;
    }

    /////////////////////////////////////////////
    ///    Earth-Sun-Jupiter, Sun is fixed    ///
    /////////////////////////////////////////////
    else if(strcmp(argv[1], "ESJ") == 0)
    {
        double factor = 1.0; // factor to scale the mass of Jupiter
        if (argc>3)
        {
            factor = atof(argv[3]);
        }
        // Creating filename
        string filename = "../benchmarks/ESJ/pos_dt"+to_string(dt)+"_N"+to_string(num_timesteps)+"_m"+to_string(factor)+".xyz";

        // Adding objects to the solar system
        solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the Sun

        // Relative masses, in units [M_sun]
        double M_sun = 2e30;             // mass of the Sun in kg
        double M_earth = 6e24/M_sun;     // rel. mass of the Earth
        double M_jupiter = 1.9e27/M_sun; // rel. mass of Jupiter

        // Adding the Earth
        vec3 pos_E = vec3(9.00219e-1, 4.36991e-1, -1.77691e-4);  // position [AU]
        vec3 vel_E = vec3(-7.76495e-3, 1.54282e-2, -4.06713e-7); // velocity [AU/day]
        solarSystem.createCelestialBody(pos_E, vel_E*365.25, M_earth); // adding the Earth

        // Adding Jupiter
        vec3 pos_J = vec3(-5.42632, -4.82222e-1, 1.23357e-1);    // position [AU]
        vec3 vel_J = vec3(5.81068e-4, -7.16029e-3, 1.67576e-5);  // velocity [AU/day]
        solarSystem.createCelestialBody(pos_J, vel_J*365.25, factor*M_jupiter); // adding Jupiter

        solarSystem.writeToFile(filename); //initializing file
        Integrator integrator(dt,method);

        //Access sun position so that we can keep it still
        // do we still want to do this in this task?
        std::vector<CelestialBody> &m_bodies = solarSystem.bodies();
        CelestialBody &body = m_bodies[0];

        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);
            body.position = vec3(0,0,0); // fixing the Sun to (0,0,0) each step
            solarSystem.writeToFile(filename);
        }

    }

    /////////////////////////////////////////////
    ///   Final model of entire solar system  ///
    /////////////////////////////////////////////
    else if(strcmp(argv[1], "WS") == 0)
    {
        // Creating filename
        string filename = "../benchmarks/WS/pos_dt"+to_string(dt)+"_N"+to_string(num_timesteps)+".xyz";

        // Relative masses, in units [M_sun]
        double M_sun = 2e30;              // mass of the Sun in kg
        double M_mercury = 2.4e23/M_sun;  // rel. mass of Mercury
        double M_venus = 4.9e24/M_sun;    // rel. mass of Venus
        double M_earth = 6e24/M_sun;      // rel. mass of the Earth
        double M_mars = 6.6e23/M_sun;     // rel. mass of Mars
        double M_jupiter = 1.9e27/M_sun;  // rel. mass of Jupiter
        double M_saturn = 5.5e26/M_sun;   // rel. mass of Saturn
        double M_uranus = 8.8e25/M_sun;   // rel. mass of Uranus
        double M_neptune = 1.03e26/M_sun; // rel. mass of Neptune
        double M_pluto = 1.31e22/M_sun;   // rel. mass of Pluto

        // Adding objects to the solar system
        vec3 pos_sun = vec3( 3.557522499727856E-03,  3.436899814085439E-03, -1.596344425455737E-04); // position [AU]
        vec3 vel_sun = vec3(-2.030857186518649E-06,  6.828431295519784E-06,  4.169177553390423E-08); // velocity [AU/day]
        solarSystem.createCelestialBody(pos_sun, vel_sun, 1.0); // adding the Sun in solar system barycenter

        // Adding Mercury
        vec3 pos_mercury = vec3(-3.879399114091706E-01, -2.710305092518798E-02,  3.326156756039202E-02); // position [AU]
        vec3 vel_mercury = vec3(-3.666413559516294E-03, -2.683296386126161E-02, -1.856913029011958E-03); // velocity [AU/day]
        solarSystem.createCelestialBody(pos_mercury, vel_mercury*365.25, M_mercury); // adding Mercury

        // Adding Venus
        vec3 pos_venus = vec3( 2.768509256072073E-01, -6.707683947636957E-01, -2.517488922555787E-02);   // position [AU]
        vec3 vel_venus = vec3( 1.860690284828718E-02,  7.536836731263053E-03, -9.705618181506076E-04);   // velocity [AU/day]
        solarSystem.createCelestialBody(pos_venus, vel_venus*365.25, M_venus); // adding Venus

        // Adding the Earth
        vec3 pos_earth = vec3( 9.002187374164167E-01, 4.369907064244718E-01, -1.776909578696060E-04);    // position [AU]
        vec3 vel_earth = vec3(-7.764951739652256E-03, 1.542821220223306E-02, -4.067127050855086E-07);    // velocity [AU/day]
        solarSystem.createCelestialBody(pos_earth, vel_earth*365.25, M_earth); // adding the Earth

        // Adding Mars
        vec3 pos_mars = vec3( 1.193657284115136E+00, -6.978436624748694E-01, -4.406298715121766E-02);     // position [AU]
        vec3 vel_mars = vec3( 7.634665383846734E-03,  1.326168661732281E-02,  9.038452721655709E-05);     // velocity [AU/day]
        solarSystem.createCelestialBody(pos_mars, vel_mars*365.25, M_mars); // adding Mars

        // Adding Jupiter
        vec3 pos_jupiter = vec3(-5.426315241208865E+00, -4.822215431711254E-01, 1.233572936426657E-01);   // position [AU]
        vec3 vel_jupiter = vec3( 5.810683650779558E-04, -7.160289109931362E-03, 1.675759064981599E-05);   // velocity [AU/day]
        solarSystem.createCelestialBody(pos_jupiter, vel_jupiter*365.25, M_jupiter); // adding Jupiter

        // Adding Saturn
        vec3 pos_saturn = vec3(-2.246529956275636E+00, -9.779906769909475E+00,  2.594563214268305E-01);   // position [AU]
        vec3 vel_saturn = vec3( 5.132048253878187E-03, -1.266819166886991E-03, -1.820892763587737E-04);   // velocity [AU/day]
        solarSystem.createCelestialBody(pos_saturn, vel_saturn*365.25, M_saturn); // adding Saturn

        // Adding Uranus
        vec3 pos_uranus = vec3( 1.845714208857284E+01,  7.575248291656612E+00, -2.109811593159080E-01);   // position [AU]
        vec3 vel_uranus = vec3(-1.522058331202997E-03,  3.455221416420103E-03,  3.261108191126768E-05);   // velocity [AU/day]
        solarSystem.createCelestialBody(pos_uranus, vel_uranus*365.25, M_uranus); // adding Uranus

        // Adding Neptune
        vec3 pos_neptune = vec3( 2.826500658751187E+01, -9.910371276589974E+00, -4.473108323922165E-01);  // position [AU]
        vec3 vel_neptune = vec3( 1.017401746439024E-03,  2.980597140971018E-03, -8.489925094644433E-05);  // velocity [AU/day]
        solarSystem.createCelestialBody(pos_neptune, vel_neptune*365.25, M_neptune); // adding Neptune

        // Adding Pluto
        vec3 pos_pluto = vec3( 9.436138126711999E+00, -3.181728263243627E+01,  6.751503215275325E-01);    // position [AU]
        vec3 vel_pluto = vec3( 3.069571857572786E-03,  2.445305051568931E-04, -9.024173240192213E-04);    // velocity [AU/day]
        solarSystem.createCelestialBody(pos_pluto, vel_pluto*365.25, M_pluto); // adding Pluto

        // Integrating the system
        solarSystem.writeToFile(filename); //initializing file
        Integrator integrator(dt,method);

        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);
            solarSystem.writeToFile(filename);
        }

    }

    else if(strcmp(argv[1], "GM") == 0)
    {
        // Creating filename
        string filename = "../benchmarks/GM/pos_dt"+to_string(dt)+"_N"+to_string(num_timesteps)+".xyz";

        // Adding objects to the solar system
        solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the Sun

        // Relative masses, in units [M_sun]
        double M_sun = 2e30;             // mass of the Sun in kg
        double M_mercury = 2.4e23/M_sun;       // mass of Mercury in kg

        // Adding the Earth
        vec3 pos_M = vec3(0.3075, 0, 0);  // position [AU]
        vec3 vel_M = vec3(0, 12.44, 0); // velocity [AU/year]
        solarSystem.createCelestialBody(pos_M, vel_M, M_mercury); // adding Mercury

        // Integrating the system
        solarSystem.writeToFile(filename); //initializing file
        Integrator integrator(dt,method);

        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);
            solarSystem.writeToFile(filename);
        }

    }
    else
    {
        cout << "For the people who are still alive." << endl;
    }


    return 0;
}


