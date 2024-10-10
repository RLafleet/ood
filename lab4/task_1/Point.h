#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point() : x(0), y(0) {}
 
    Point(double xValue, double yValue) : x(xValue), y(yValue) {}

    double x;
    double y;
};


#endif //POINT_H