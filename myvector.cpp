#include "myvector.h"
#include <math.h>
#include <QVector3D>
MyVector::MyVector(double xx, double yy, double zz)
{
    xPoint = xx;
    yPoint = yy;
    zPoint = zz;
}


QVector3D MyVector::to_qvector()
{
    return QVector3D(xPoint, yPoint, zPoint);
}

MyVector::~MyVector()
{
}

void MyVector::set_x(double x)
{
    xPoint = x;
}

double MyVector::get_x() const
{
    return xPoint;
}

void MyVector::set_y(double y)
{
    yPoint = y;
}

double MyVector::get_y() const
{
    return yPoint;
}

void MyVector::set_z(double z)
{
    zPoint = z;
}

double MyVector::get_z() const
{
    return zPoint;
}

void MyVector::normalize()
{
    double mag{magnitude()};
    if(mag != 0)
    {
        xPoint = xPoint/mag;
        yPoint = yPoint/mag;
        zPoint = zPoint/mag;
    }

}

double dot(MyVector &v1, MyVector &v2)
{
    return v2.get_x()*v1.get_x() + v2.get_y()*v1.get_y() + v2.get_z()*v1.get_z();
}

MyVector cross(MyVector &v1, MyVector &v2)
{
    double i{v1.get_y()*v2.get_z() - v1.get_z()*v2.get_y()};
    double j{v1.get_z()*v2.get_x() - v1.get_x()*v2.get_z()};
    double k{v1.get_x()*v2.get_y() - v1.get_y()*v2.get_x()};
    return MyVector(i, j, k);
}

double MyVector::magnitude()
{
    return sqrt( pow(xPoint, 2) + pow(yPoint, 2) + pow(zPoint, 2));
}

double MyVector::distance_between(MyVector &v1, MyVector &v2)
{
    MyVector temp{v1-v2};
    return temp.magnitude();
}

MyVector operator+(const MyVector &v1, const MyVector &v2)
{
    double x{v1.get_x()+v2.get_x()};
    double y{v1.get_y()+v2.get_y()};
    double z{v1.get_z()+v2.get_z()};
    return MyVector(x, y, z);
}

MyVector operator-(const MyVector &v1, const MyVector &v2)
{
    double x{v1.get_x()-v2.get_x()};
    double y{v1.get_y()-v2.get_y()};
    double z{v1.get_z()-v2.get_z()};
    return MyVector(x, y, z);
}

MyVector operator*(const MyVector &v1, const int &number)
{
    return MyVector(v1.get_x()*number, v1.get_y()*number, v1.get_z()*number);
}

MyVector operator*(const MyVector &v1, const float &number)
{
    return MyVector(v1.get_x()*number, v1.get_y()*number, v1.get_z()*number);
}

MyVector operator*(const MyVector &v1, const double &number)
{
    return MyVector(v1.get_x()*number, v1.get_y()*number, v1.get_z()*number);
}

MyVector operator*(const int &number, const MyVector &v1)
{
    return MyVector(v1.get_x()*number, v1.get_y()*number, v1.get_z()*number);
}

MyVector operator*(const float &number, const MyVector &v1)
{
    return MyVector(v1.get_x()*number, v1.get_y()*number, v1.get_z()*number);
}

MyVector operator*(const double &number, const MyVector &v1)
{
    return MyVector(v1.get_x()*number, v1.get_y()*number, v1.get_z()*number);
}

MyVector & MyVector::operator+=(const MyVector &v1)
{
    this->set_x(this->get_x()+v1.get_x());
    this->set_y(this->get_y()+v1.get_y());
    this->set_z(this->get_z()+v1.get_z());
    return *this;
}

MyVector & MyVector::operator/=(const int &digit)
{
    this->set_x(this->get_x() / digit);
    this->set_y(this->get_y() / digit);
    this->set_z(this->get_z() / digit);
    return *this;
}

MyVector & MyVector::operator/=(const float &digit)
{
    this->set_x(this->get_x() / digit);
    this->set_y(this->get_y() / digit);
    this->set_z(this->get_z() / digit);
    return *this;
}

MyVector & MyVector::operator/=(const double &digit)
{
    this->set_x(this->get_x() / digit);
    this->set_y(this->get_y() / digit);
    this->set_z(this->get_z() / digit);
    return *this;
}

bool operator==(const MyVector &v1, const MyVector &v2)
{
    if( (v1.get_x() == v2.get_x()) && (v1.get_y() == v2.get_y()) && (v1.get_z() == v1.get_z()) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool operator!=(const MyVector &v1, const MyVector &v2)
{
    if( (v1.get_x() == v2.get_x()) && (v1.get_y() == v2.get_y()) && (v1.get_z() == v1.get_z()) )
    {
        return false;
    }
    else
    {
        return true;
    }
}
