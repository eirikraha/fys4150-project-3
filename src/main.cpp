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
#include "integrator.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 5){
        cout<<"Usage: "<<argv[0]<<" Task"<<" dt"<<" task_param"<<" Method"<<endl;
        cout<<"Task and dt always required, task_param and Method optional and depends on Task."<<"\n\n";
        cout<<"   Task : ES, ESJ, WS or GR"<<endl;
        cout<<"   ES : Earth-Sun system. The Sun is fixed to origo."<<endl;
        cout<<"   ESJ1 : Earth-Sun-Jupiter system. The Sun is fixed to origo."<<endl;
        cout<<"   ESJ2 : Earth-Sun-Jupiter system. Sun not fixed to origo, total momentum fixed to 0."<<endl;
        cout<<"   WS : Whole solar system, including Pluto. The Sun is not fixed to origo."<<endl;
        cout<<"   GR : Sun-Mercury system with a general relativity correction to the gravitational force."<<endl;
        cout<<"   dt : time step" << endl;
        cout<<"   task_param : Meaning depends on Task"<<endl;
        cout<<"     -ES : task_param is initial velocity of the Earth in y direction [AU/yr]."<<endl;
        cout<<"     -ESJ1 and ESJ2: task_param spesifies a number multiplied with mass of Jupiter to alter it."<<endl;
        cout<<"     -WS and GR: task_param spesifies number of years for simulation to go through" <<endl;
        cout<<"   Method : Euler, Verlet or Verlet with a general relativity term, set to Verlet if not specified" << endl;
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
    int max_steps = 1000; // max steps to save to file
    int iter=1;
    int num_timesteps = 1000;

    /////////////////////////////////////////////
    ///    Earth-Sun system, Sun is fixed     ///
    /////////////////////////////////////////////
    if (strcmp(argv[1], "ES") == 0)
    {
        double v_y0 = 6.28; // for circular orbit
        if (argc > 3)
        {
            v_y0 = atof(argv[3]);
        }

        // Finding number of time steps based on how many years we want the program to run
        int years = 1;
        num_timesteps = years/dt; //  yr / dt [yr]
        if (num_timesteps > max_steps) iter = num_timesteps/max_steps;

        // Creating filename for this task
        string meth = method;
        string filename = "../benchmarks/ES/pos_"+meth+"_dt"+to_string(dt)+"_yrs"+to_string(years)+"_vy"+to_string(v_y0)+".xyz";

        // For timing segments of code, comment out write to file
        //clock_t start, finish;
        //ofstream time_ofile;
        //time_ofile.open("../benchmarks/ES/timing.txt",ios_base::app);

        // Adding objects to our solar system
        solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the sun

        vec3 r_earth(1,0,0);
        vec3 v_earth(0,v_y0,0);
        solarSystem.createCelestialBody(r_earth, v_earth, 3e-6); // adding the Earth

        solarSystem.writeToFile(filename); //initializing file
        Integrator integrator(dt,method);

        //Access sun position so that we can keep it still
        vector<CelestialBody> &m_bodies = solarSystem.bodies();
        CelestialBody &body = m_bodies[0];

        // Checking if kinetic and potential energy is conserved
        double totalEnergy = solarSystem.totalEnergy();
        vec3 momentum = solarSystem.momentum();
        vec3 angularMomentum = solarSystem.angularMomentum();

        //start = clock();
        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);
            body.position = vec3(0,0,0); // fixing the Sun to (0,0,0) each step

            if (i%iter == 0) solarSystem.writeToFile(filename); // only write to file every iter steps

            if (i%((int)(0.1*num_timesteps)) == 0)
            {
                totalEnergy = solarSystem.totalEnergy();
                momentum = solarSystem.momentum();
                angularMomentum = solarSystem.angularMomentum();
                cout<<i<<endl<<"E_tot = "<<totalEnergy<<endl;
                cout<<"p = "<<momentum.length()<<endl;
                cout<<"L = "<<angularMomentum.length()<<endl;
            }

        }

        // Time calculation and write to file
        //finish = clock();
        //double time_duration = ((finish-start)/((double)CLOCKS_PER_SEC));
        //time_ofile<<method<<", dt = "<<setw(8)<<dt<<", N = "<<setw(8)<<num_timesteps<<endl;
        //time_ofile<<"t = "<<setw(10)<<setprecision(8)<<time_duration<<"\n\n";
        //time_ofile.close();

    }

    /////////////////////////////////////////////
    ///           Earth-Sun-Jupiter           ///
    /////////////////////////////////////////////
    else if(strcmp(argv[1], "ESJ1") == 0 || strcmp(argv[1], "ESJ2") == 0)
    {
        double factor = 1.0; // factor to scale the mass of Jupiter
        if (argc>3)
        {
            factor = atof(argv[3]);
        }
        int years = 12;  // 12 years a solar system
        num_timesteps = years/dt;  // finding number of time steps
        if (num_timesteps > max_steps) iter = num_timesteps/max_steps;

        // Creating filename
        string filename = "../benchmarks/"+(string)argv[1]+"/pos_dt"+to_string(dt)+"_yrs"+to_string(years)+"_m"+to_string(factor)+".xyz";

        // Adding objects to the solar system
        solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the Sun

        // Relative masses, in units [M_sun]
        double M_sun = 2e30;             // mass of the Sun in kg
        double M_earth = 6e24/M_sun;     // rel. mass of the Earth
        double M_jupiter = 1.9e27/M_sun; // rel. mass of Jupiter

        // Adding the Earth
        vec3 pos_earth = vec3( 9.002187374164167E-01, 4.369907064244718E-01, -1.776909578696060E-04);    // position [AU]
        vec3 vel_earth = vec3(-7.764951739652256E-03, 1.542821220223306E-02, -4.067127050855086E-07);    // velocity [AU/day]
        solarSystem.createCelestialBody(pos_earth, vel_earth*365.25, M_earth); // adding the Earth

        // Adding Jupiter
        vec3 pos_jupiter = vec3(-5.426315241208865E+00, -4.822215431711254E-01, 1.233572936426657E-01);   // position [AU]
        vec3 vel_jupiter = vec3( 5.810683650779558E-04, -7.160289109931362E-03, 1.675759064981599E-05);   // velocity [AU/day]
        solarSystem.createCelestialBody(pos_jupiter, vel_jupiter*365.25, factor*M_jupiter); // adding Jupiter

        solarSystem.writeToFile(filename); //initializing file
        Integrator integrator(dt,method);

        //Access sun position so that we can keep it still
        vector<CelestialBody> &m_bodies = solarSystem.bodies();
        CelestialBody &body = m_bodies[0];

        // Making sure the momentum is zero, correcting the velocity of the Sun
        if (strcmp(argv[1],"ESJ2") == 0)
        {
            // Placing the sun in the full solar system barycenter
            vec3 pos_sun = vec3( 3.557522499727856E-03,  3.436899814085439E-03, -1.596344425455737E-04); // position [AU]
            vec3 vel_sun = vec3(-2.030857186518649E-06,  6.828431295519784E-06,  4.169177553390423E-08); // velocity [AU/day]
            body.position = pos_sun;
            body.velocity = vel_sun;

            vec3 mom_tot = vec3(0,0,0);
            vec3 mom_sun = body.mass*body.velocity;
            for (unsigned int i=1; i<m_bodies.size(); i++)
            {
                mom_tot += m_bodies[i].mass*m_bodies[i].velocity;
            }
            body.velocity -= (mom_sun + mom_tot)/body.mass;
        }

        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);
            if (strcmp(argv[1],"ESJ1")==0) body.position = vec3(0,0,0); // fixing the Sun to (0,0,0) each step
            if (i%iter == 0) solarSystem.writeToFile(filename); // only write to file every iter steps
        }

    }

    /////////////////////////////////////////////
    ///   Final model of entire solar system  ///
    /////////////////////////////////////////////
    else if(strcmp(argv[1], "WS") == 0)
    {
        double years = 100; // the amount of years we want the program to run over
        if (argc>3)
        {
            years = atof(argv[3]);
        }
        num_timesteps = (int)(years/dt);
        max_steps = 100000;
        if (num_timesteps > max_steps) iter = num_timesteps/max_steps;

        // Creating filename
        string filename = "../benchmarks/WS/pos_dt"+to_string(dt)+"_yrs"+to_string(years)+".xyz";

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
        solarSystem.createCelestialBody(pos_sun, vel_sun*365.25, 1.0); // adding the Sun in solar system barycenter

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

        // Making sure the momentum is zero, correcting the velocity of the Sun
        vector<CelestialBody> &m_bodies = solarSystem.bodies();
        vec3 mom_tot = vec3(0,0,0);
        vec3 mom_sun = m_bodies[0].mass*m_bodies[0].velocity;
        for (unsigned int i=1; i<m_bodies.size(); i++)
        {
            mom_tot += m_bodies[i].mass*m_bodies[i].velocity;
        }
        m_bodies[0].velocity -= (mom_sun + mom_tot)/m_bodies[0].mass;

        // Checking if kinetic and potential energy, momentum, and angular momentum is conserved
        double totalEnergy = solarSystem.totalEnergy();
        vec3 momentum = solarSystem.momentum();
        vec3 angularMomentum = solarSystem.angularMomentum();

        // Integrating the system
        solarSystem.writeToFile(filename); //initializing file
        Integrator integrator(dt,method);

        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);
            if (i%iter == 0) solarSystem.writeToFile(filename); // only write to file every iter steps

            /*if (i%((int)(0.1*num_timesteps)) == 0)
            *{
            *    totalEnergy = solarSystem.totalEnergy();
            *    momentum = solarSystem.momentum();
            *    angularMomentum = solarSystem.angularMomentum();
            *    cout<<i<<endl<<"E_tot = "<<totalEnergy<<endl;
            *    cout<<"p = "<<momentum.length()<<endl;
            *    cout<<"L = "<<angularMomentum.length()<<endl;
            *}
            */
        }

    }

    /////////////////////////////////////////////
    ///   Mercury-Sun, general relativity     ///
    /////////////////////////////////////////////
    else if(strcmp(argv[1], "GR") == 0)
    {
        double years = 100; // the amount of years we want the program to run over
        if (argc>3)
        {
            years = atof(argv[3]);
        }
        num_timesteps = (int)(years/dt);

        //makes dt a string on scientific form
        stringstream ss;
        ss << scientific << setprecision(1) << dt;
        string dtstr = ss.str();

        // Creating filename
        string methodstr = method;
        string filename = "../benchmarks/GR/ang_per"+methodstr + "_dt" + dtstr + "_years" + to_string(years);

        // Adding objects to the solar system
        solarSystem.createCelestialBody(vec3(0,0,0),vec3(0,0,0),1.0); // adding the Sun

        // Relative masses, in units [M_sun]
        double M_sun = 2e30;             // mass of the Sun in kg
        double M_mercury = 2.4e23/M_sun;       // mass of Mercury in kg

        // Adding Mercury
        vec3 pos_M = vec3(0.3075, 0, 0);  // position [AU]
        vec3 vel_M = vec3(0, 12.44, 0); // velocity [AU/year]
        solarSystem.createCelestialBody(pos_M, vel_M, M_mercury); // adding Mercury

        // Set some helper variables before we start the time integration.
        double thetaPrevious        = 0;	// The perihelion angle of the previous time step.
        double thetaCurrent         = 0;	// The perihelion angle of the current time step.

        double rPreviousPrevious 	= 0;	// Mercury-Sun-distance two times steps ago.
        double rPrevious            = 0;	// Mercury-Sun-distance of the previous time step.
        double rCurrent             = 0;	// Mercury-Sun-distance of the current time step.

        vector<CelestialBody> &m_bodies = solarSystem.bodies();

        // Integrating the system
        solarSystem.writeToFilePER(filename, 0, thetaPrevious); //initializing file
        Integrator integrator(dt,method);

        for (int i=0; i<num_timesteps; i++)
        {
            integrator.integrateOneStep(solarSystem);

            // Compute the current perihelion angle, using the inverse tangent function from cmath.
            // This assumes there is a vector of planets, called m_bodies, available, in which the
            // Sun is m_bodies[0] and Mercury is m_bodies[1].
            double x = m_bodies[1].position.x() - m_bodies[0].position.x();
            double y = m_bodies[1].position.y() - m_bodies[0].position.y();
            thetaCurrent = atan2( y, x );

            // Compute the current Mercury-Sun distance.
            rCurrent = (m_bodies[1].position - m_bodies[0].position).length();

            // Check if the *previous* time step was a minimum for the Mercury-Sun distance. I.e. check
            // if the previous distance is smaller than the current one *and* the previous previous one.
            if ( rCurrent > rPrevious && rPrevious < rPreviousPrevious )
                {
                    // Here you should also probably write it to file for later plotting or something.
                    solarSystem.writeToFilePER(filename, i, thetaPrevious); //writing to file
                 }

             // Update the helper variables (current, previous, previousPrevious).
             rPreviousPrevious   = rPrevious;
             rPrevious           = rCurrent;
             thetaPrevious       = thetaCurrent;
        }

    }
    else
    {
        cout << "For the people who are still alive." << endl;
    }


    return 0;
}


