# fys4150-project-3
Folder for all programs and files used in project 3

A differential equation solver specified to simulate the solar system using forward Euler and Velocity Verlet.

How to run:
Command line arguments: Task, dt, task_param and method.
Task and dt always required, task_param and Method optional and depends on Task.

   Task : ES, ESJ, WS or GR
   ES : Earth-Sun system. The Sun is fixed to origo.
   ESJ1 : Earth-Sun-Jupiter system. The Sun is fixed to origo.
   ESJ2 : Earth-Sun-Jupiter system. Sun not fixed to origo, total momentum fixed to 0.
   WS : Whole solar system, including Pluto. The Sun is not fixed to origo.
   GR : Sun-Mercury system with a general relativity correction to the gravitational force.
   dt : time step
   task_param : Meaning depends on Task
     -ES : task_param is initial velocity of the Earth in y direction [AU/yr].
     -ESJ: task_param spesifies a number multiplied with mass of Jupiter to alter it.
   Method : Euler, Verlet or Verlet with a general relativity term, set to Verlet if not specified
    Note that Verlet is actually velocity Verlet, but the name is fairly long, hence the shortening.
