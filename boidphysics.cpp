#include "boidphysics.h"
#include "myvector.h"
#include <random>

BoidPhysics::BoidPhysics(MyVector pos, MyVector orient)
{
    position = pos;
    orientation = orient;
    newPosition = pos;
    newOrientation = orient;
    orientation.normalize();
    newOrientation.normalize();
}

BoidPhysics::BoidPhysics()
{
    position = BoidPhysics::random_position();
    orientation = BoidPhysics::random_vector_minus1_1();
    newOrientation = BoidPhysics::random_vector_minus1_1();
    newOrientation.normalize();
    orientation.normalize();
}

MyVector BoidPhysics::random_vector_minus1_1()
{
    return MyVector(double((rand() % 100)/50-1), double((rand() % 100)/50-1), double((rand() %100)/50-1));
}

MyVector BoidPhysics::random_position()
{
    return MyVector(MyVector(double(rand() % 20), double(rand() % 20), double(rand() %20)));
}

void BoidPhysics::calculate_new_position(double speed, int timeStep)
{
    newPosition = position + orientation*(speed*timeStep);
}

void BoidPhysics::calculate_new_orietation(double alignmentWeight, double separationWeight, double cohesionWeight)
{
    newOrientation = alignment*alignmentWeight + separation*separationWeight + cohesion*cohesionWeight;
//    newOrientation = alignment*alignmentWeight + separation*separationWeight + cohesion*cohesionWeight +turnFactor;
    newOrientation.normalize();
    if(neightborCount == 0)
    {
        newOrientation = orientation;
    }
}

void BoidPhysics::update_position_and_orientation()
{
    position = newPosition;
    orientation = newOrientation;
}

void BoidPhysics::compute_all(std::vector<BoidPhysics*> &boidsVector, double radius)
{
    neightborCount = 0;
    MyVector center{MyVector(0, 0, 0)};
    alignment = orientation;
    for(int i{0}; i<boidsVector.size(); i++)
    {
        if(position != boidsVector[i]->position)
        {
            if(MyVector::distance_between(position, boidsVector[i]->position) < radius )
            {
                neightborCount++;
                center += boidsVector[i]->position;
                alignment += boidsVector[i]->orientation;
            }
        }
    }
    if(neightborCount != 0)
    {
        center /= neightborCount;
        cohesion = center-position;
        separation = position - center;
    }
    else
    {
        cohesion = MyVector(0, 0, 0);
        separation = MyVector(0, 0, 0);
    }
    separation.normalize();
    cohesion.normalize();
    alignment.normalize();
//    check_box_position();
}

void BoidPhysics::check_x_wall()
{
    if((newPosition.get_x()>maxPoint-beginDetectWall))
    {
        newOrientation.set_x(newOrientation.get_x()-turnFactor);
        if(newPosition.get_x()>maxPoint)
        {
            newPosition.set_x(maxPoint);
        }
    }
    else if((newPosition.get_x()<minPoint+beginDetectWall))
    {
        newOrientation.set_x(newOrientation.get_x()+turnFactor);
        if(newPosition.get_x()<minPoint)
        {
            newPosition.set_x(minPoint);
        }
    }
}

void BoidPhysics::check_y_wall()
{
    if((newPosition.get_y()>maxPoint-beginDetectWall))
    {
            newOrientation.set_y(newOrientation.get_y()-turnFactor);
            if(newPosition.get_y()>maxPoint)
            {
                newPosition.set_y(maxPoint);
            }
    }
    else if((newPosition.get_y()<minPoint+beginDetectWall))
    {
        newOrientation.set_y(newOrientation.get_y()+turnFactor);
        if(newPosition.get_y()<minPoint)
        {
            newPosition.set_y(minPoint);
        }
    }
}

void BoidPhysics::check_z_wall()
{
    if((newPosition.get_z()>maxPoint-beginDetectWall))
    {
        newOrientation.set_z(newOrientation.get_z()-turnFactor);
        if(newPosition.get_z()>maxPoint)
        {
            newPosition.set_z(maxPoint);
        }
    }
    else if((newPosition.get_z()<minPoint+beginDetectWall))
    {
        newOrientation.set_z(newOrientation.get_z()+turnFactor);
        if(newPosition.get_z()<minPoint)
        {
            newPosition.set_z(minPoint);
        }
    }
}

//void BoidPhysics::check_x_wall()
//{
//    if((position.get_x()>maxPoint-beginDetectWall))
//    {
//        turnFactor.set_x(turnFactor.get_x()-1);
//    }
//    else if((position.get_x()<minPoint+beginDetectWall))
//    {
//       turnFactor.set_x(turnFactor.get_x()+1);
//    }
//}

//void BoidPhysics::check_y_wall()
//{
//    if((position.get_y()>maxPoint-beginDetectWall))
//    {
//        turnFactor.set_y(turnFactor.get_y()-1);
//    }
//    else if((position.get_y()<minPoint+beginDetectWall))
//    {
//        turnFactor.set_y(turnFactor.get_y()+1);
//    }
//}

//void BoidPhysics::check_z_wall()
//{
//    if((position.get_z()>maxPoint-beginDetectWall))
//    {
//        turnFactor.set_z(turnFactor.get_z()-1);
//    }
//    else if((position.get_z()<minPoint+beginDetectWall))
//    {
//        turnFactor.set_z(turnFactor.get_z()+1);
//    }
//}

void BoidPhysics::check_box_position()
{
    check_x_wall();
    check_z_wall();
    check_y_wall();
    newOrientation.normalize();
//    turnFactor.normalize();
}
