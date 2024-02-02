#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <QtCore>
#include <QVector3D>

class MyVector
{

public:
    MyVector(double xx, double yy, double zz);
    ~MyVector();

    double xPoint{0};
    double yPoint{0};
    double zPoint{0};

    void set_x(double x);
    double get_x() const;
    void set_y(double x);
    double get_y() const;
    void set_z(double x);
    double get_z() const;
    void normalize();

    QVector3D to_qvector();

    static double dot(MyVector &v1, MyVector &v2);
    static MyVector cross(MyVector &v1, MyVector &v2);
    double magnitude();
    static double distance_between(MyVector &v1, MyVector &v2);

    friend MyVector operator+(const MyVector &v1, const MyVector &v2);
    friend MyVector operator-(const MyVector &v1, const MyVector &v2);
    friend MyVector operator*(const MyVector &v1, const int &number);
    friend MyVector operator*(const MyVector &v1, const float &number);
    friend MyVector operator*(const MyVector &v1, const double &number);
    friend MyVector operator*(const int &number, const MyVector &v1);
    friend MyVector operator*(const float &number, const MyVector &v1);
    friend MyVector operator*(const double &number, const MyVector &v1);
    MyVector & operator+=(const MyVector &v1);
    MyVector & operator/=(const int &digit);
    MyVector & operator/=(const float &digit);
    MyVector & operator/=(const double &digit);

    friend bool operator==(const MyVector &v1, const MyVector &v2);
    friend bool operator!=(const MyVector &v1, const MyVector &v2);


private:

protected:
    double mX{0};
};

#endif // MYVECTOR_H
