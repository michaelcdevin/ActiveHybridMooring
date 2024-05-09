#include <stdio.h>
#include <stdlib.h>
#include <moordyn/MoorDyn2.h>

int main(int, char**)
{
    // int err;
    printf("YEET YEET");
    // MoorDyn system = MoorDyn_Create("C:/code/ActiveHybridMooring/MoorDyn/oc3.txt");
    // if (!system)
    //     return 1;

    // // 3 coupled points x 3 components per point = 9 DoF
    // double x[9], xd[9];
    // memset(xd, 0.0, sizeof(double));
    // // Get the initial positions from the system itself
    // for (unsigned int i = 0; i < 3; i++) {
    //     // 4 = first fairlead id
    //     MoorDynPoint point = MoorDyn_GetPoint(system, i + 4);
    //     err = MoorDyn_GetPointPos(point, x + 3 * i);
    //     if (err != MOORDYN_SUCCESS) {
    //         MoorDyn_Close(system);
    //         return 1;
    //     }
    // }

    // // Setup the initial condition
    // err = MoorDyn_Init(system, x, xd);
    // if (err != MOORDYN_SUCCESS) {
    //     MoorDyn_Close(system);
    //     return 1;
    // }

    // // Make the points move at 0.5 m/s to the positive x direction
    // for (unsigned int i = 0; i < 3; i++)
    //     xd[3 * i] = 0.5;
    // double t = 0.0, dt = 0.5;
    // double f[9];
    // err = MoorDyn_Step(system, x, xd, f, &t, &dt);
    // if (err != MOORDYN_SUCCESS) {
    //     MoorDyn_Close(system);
    //     return 1;
    // }

    // // Print the position and tension of the line nodes
    // unsigned int n_lines;
    // err = MoorDyn_GetNumberLines(system, &n_lines);
    // if (err != MOORDYN_SUCCESS) {
    //     MoorDyn_Close(system);
    //     return 1;
    // }
    // for (unsigned int i = 0; i < n_lines; i++) {
    //     const unsigned int line_id = i + 1;
    //     printf("Line %u\n", line_id);
    //     printf("=======\n");
    //     MoorDynLine line = MoorDyn_GetLine(system, line_id);
    //     if (!line) {
    //         MoorDyn_Close(system);
    //         return 1;
    //     }
    //     unsigned int n_nodes;
    //     err = MoorDyn_GetLineNumberNodes(line, &n_nodes);
    //     if (err != MOORDYN_SUCCESS) {
    //         MoorDyn_Close(system);
    //         return 1;
    //     }
    //     for (unsigned int j = 0; j < n_nodes; j++) {
    //         printf("  node %u:\n", j);
    //         double pos[3], ten[3];
    //         err = MoorDyn_GetLineNodePos(line, j, pos);
    //         if (err != MOORDYN_SUCCESS) {
    //             MoorDyn_Close(system);
    //             return 1;
    //         }
    //         printf("  pos = [%g, %g, %g]\n", pos[0], pos[1], pos[2]);
    //         err = MoorDyn_GetLineNodeTen(line, j, ten);
    //         if (err != MOORDYN_SUCCESS) {
    //             MoorDyn_Close(system);
    //             return 1;
    //         }
    //         printf("  ten = [%g, %g, %g]\n", ten[0], ten[1], ten[2]);
    //     }
    // }

    // // Alright, time to finish!
    // err = MoorDyn_Close(system);
    // if (err != MOORDYN_SUCCESS)
    //     return 1;

    return 0;
}