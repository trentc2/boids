#ifndef BOIDPHYSICS_H
#define BOIDPHYSICS_H

#include "myvector.h"


class BoidPhysics
{
public:
    BoidPhysics(MyVector pos, MyVector orient);
    BoidPhysics();


    MyVector position{0.0, 0.0, 0.0};
    MyVector newPosition{0.0, 0.0, 0.0};
    MyVector orientation{0.0, 0.0, 0.0};
    MyVector newOrientation{0.0, 0.0, 0.0};

    MyVector separation{0.0, 0.0, 0.0};
    MyVector cohesion{0.0, 0.0, 0.0};
    MyVector alignment{0.0, 0.0, 0.0};
//    MyVector turnFactor{0.0, 0.0, 0.0};

    int neightborCount{0};
    double maxPoint{20};
    double minPoint{0};
    double beginDetectWall{2};
    double turnFactor{.5};

    void calculate_new_position(double speed, int timeStep);
    void update_position_and_orientation();
    void calculate_new_orietation(double alignmentWeight, double separationWeight, double cohesionWeight);
    void check_box_position();
    void check_x_wall();
    void check_y_wall();
    void check_z_wall();

    void compute_all(std::vector<BoidPhysics*> &boidsVector, double radius);
    MyVector random_vector_minus1_1();
    MyVector random_position();
};

#endif // BOIDPHYSICS_H
