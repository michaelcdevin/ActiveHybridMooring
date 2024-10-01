#include <iostream>
#include <cmath>
#include "Config.h"
#include "MoorDyn2.hpp"

double amp = 5.0;
double per = 20;
inline constexpr double pi = 3.14159265358979323846;

int main(int, char**) {
    // define system
    auto system = new moordyn::MoorDyn("../../MoorDyn/ahm_3dof_dynamic_relax.txt"); //("../../MoorDyn/oc3.txt");
    if (!system)
        return 1;
    
    // get general system definitions
    auto points = system->GetPoints();
    int num_points = points.size();
    auto lines = system->GetLines();
    int num_lines = lines.size();
    const int ndof = system->NCoupledDOF();
    double x[ndof], xd[ndof], f[ndof];

    // get initial points
    for (size_t i=0; i < num_lines; i++) {
        auto init_pts = points[i*2]->getPosition(); // even #s are partition pts, odd #s are anchors
        x[i*3] = static_cast<double>(init_pts[0]);
        x[i*3+1] = static_cast<double>(init_pts[1]);
        x[i*3+2] = static_cast<double>(init_pts[2]);
    }

    // initial velocity is zero
    memset(xd, 0.0, sizeof(double));
    
    // Setup for time series simulation
    int err = system->Init(x, xd);
    if (err != MOORDYN_SUCCESS) {
        err = MoorDynClose();
        return 1;
    }

    double t_start = 0.0, dt = 0.05, t_end = 1.0;
    // TIME MARCHING: move points along sine wave and print all resulting tensions
    moordyn::vec pos;
    for (double t = t_start; t <= t_end; t += dt) {
        for (unsigned int i = 0; i < num_lines; i++) {
            x[3 * i] += amp * sin(2*pi / per * t);
        }
        err = system->Step(x, xd, f, t, dt);
        if (err != MOORDYN_SUCCESS) {
            MoorDynClose();
            return 1;
        }
        

        for (size_t i=0; i < ndof; i++) {
            std::cout << f[i] << "\t";
        }
        std::cout << std::endl;

    }
    
    // Close program
    err = MoorDynClose();
    if (err != MOORDYN_SUCCESS)
        return 1;

    return 0;
}