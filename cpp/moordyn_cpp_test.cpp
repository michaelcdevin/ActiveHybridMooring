#include <iostream>
#include <sstream>
#include "Config.h"
#include "MoorDyn2.hpp"

using namespace moordyn;

int main(int, char**)
{
    // define system
    auto system = new MoorDyn("../../MoorDyn/oc3.txt");
    if (!system)
        return 1;
    
    // get general system properties
    auto points = system->GetPoints();
    int num_points = points.size();
    auto lines = system->GetLines();
    int num_lines = lines.size();

    // 3 components per point
    double x[system->NCoupledDOF()], xd[system->NCoupledDOF()];

    // initial velocity is zero
    memset(xd, 0.0, sizeof(double));

    // get the initial positions
    for (unsigned int i = 0; i < num_points; i++) {

    }
    // Setup the initial condition
    int err = system->Init(x, xd);
    if (err != MOORDYN_SUCCESS) {
        MoorDynClose();
        return 1;
    }

    // Make the points move at 0.5 m/s to the positive x direction
    for (unsigned int i = 0; i < 3; i++)
        xd[3 * i] = 0.5;
    double t = 0.0, dt = 0.5;
    double f[9];
    err = system->Step(x, xd, f, t, dt);
    if (err != MOORDYN_SUCCESS) {
        MoorDynClose();
        return 1;
    }

    // Print the position and tension of the line nodes
    
    for (unsigned int i = 0; i < lines.size(); i++) {
        Line line = *lines[i];
        const unsigned int line_id = i + 1;
        std::cout << "Line " << line.lineId << "\n";
        std::cout << "=======" << std::endl;
        unsigned int n_nodes = line.getN() + 1;
        for (unsigned int j = 0; j < n_nodes; j++) {
            std::cout << "  node " << j << ":" << std::endl;
            vec pos = line.getNodePos(j);
            std::cout << "  pos = [" << pos << "]" << std::endl;
            vec ten = line.getNodeTen(j);
            std::cout << "  ten = [" << ten << "]" << std::endl;
        }
    }

    // Close program
    err = MoorDynClose();
    if (err != MOORDYN_SUCCESS)
        return 1;

    return 0;
}