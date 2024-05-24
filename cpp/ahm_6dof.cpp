#include <iostream>
#include <cmath>
#include "Config.h"
#include "MoorDyn2.hpp"

double amp = 5.0;
double per = 20;
inline constexpr double pi = 3.14159265358979323846;

int main(int, char**) {
    // define system
    auto system = new moordyn::MoorDyn("../../MoorDyn/ahm_6dof.txt"); //("../../MoorDyn/oc3.txt");
    if (!system)
        return 1;
    // get general system definitions
    auto points = system->GetPoints();
    int num_points = points.size();
    auto lines = system->GetLines();
    int num_lines = lines.size();
    auto bodies = system->GetBodies();
    int num_bodies = bodies.size();
    const int ndof = system->NCoupledDOF();
    double x[ndof], xd[ndof], f[ndof];

    // initial positions -- this ideally would be read from the input file TODO
    x[0] =  -407.060;
    x[1] =     0.000;
    x[2] =  -446.000;
    x[3] =     0.000;
    x[4] =     0.000;
    x[5] =   180.000;
    x[6] =   148.490;
    x[7] =   387.691;
    x[8] =  -446.000;
    x[9] =     0.000;
    x[10] =    0.000;
    x[11] =   60.000;
    x[12] =  148.490;
    x[13] = -387.691;
    x[14] = -446.000;
    x[15] =    0.000;
    x[16] =    0.000;
    x[17] =  -60.000;

    // for (size_t i=0; i < num_bodies; i++) {
    //     auto init_pos = bodies[i]->getPosition();
    //     auto init_ang = bodies[i]->getAngles();
    //     std::cout << init_pos << std::endl;
    //     std::cout << init_ang << std::endl;
    //     // double* pts = &init_pts;
    //     // std::copy(std::begin(pts), std::end(pts), x[3*i]);
    //     x[i*6] = static_cast<double>(init_pos[0]);
    //     x[i*6+1] = static_cast<double>(init_pos[1]);
    //     x[i*6+2] = static_cast<double>(init_pos[2]);
    //     x[i*6+3] = static_cast<double>(init_ang[0]);
    //     x[i*6+4] = static_cast<double>(init_ang[1]);
    //     x[i*6+5] = static_cast<double>(init_ang[2]);
    // }
    
    // initial velocity is zero
    memset(xd, 0.0, sizeof(double));
    
    // Setup for time series simulation
    int err = system->Init(x, xd);
    if (err != MOORDYN_SUCCESS) {
        err = MoorDynClose();
        return 1;
    }
    double t_start = 0.0, dt = 0.05, t_end = 1.0;
    // TIME MARCHING: move bodies along sine wave and print all resulting tensions
    moordyn::vec pos;
    for (double t = t_start; t <= t_end; t += dt) {
        for (unsigned int i = 0; i < num_bodies; i++) {
            x[6 * i] += amp * sin(2*pi / per * t);
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