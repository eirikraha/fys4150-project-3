#ifndef INTEGRATOR_H
#define INTEGRATOR_H


class Integrator
{
public:
    double m_dt;
    char const *num_method;
    Integrator(double dt, const char *method);
    void integrateOneStep(class SolarSystem &system);
};

#endif // INTEGRATOR_H
