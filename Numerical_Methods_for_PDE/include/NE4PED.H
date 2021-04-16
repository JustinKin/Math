#ifndef __NE4PED_H__
#define __NE4PED_H__

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include "D:\Lib\Eigen3\Eigen\Eigen"

class Point
{
public:
    double x;
    double y;
    double value;
    Point() : x(3.14), y(3.14), value(3.14) {}
    ~Point(){}
    Point(double x_, double y_) : x(x_), y(y_), value(3.14) {}
    Point(double x_, double y_, double value_) : x(x_), y(y_), value(value_) {}
    Point(const Point& p) : x(p.x), y(p.y), value(p.value) {}
};

class Triangle
{
public:
    Point i;
    Point j;
    Point k;
    Triangle(Point i_, Point j_, Point k_) : i(i_), j(j_), k(k_){}
    ~Triangle(){}
};

//P82, 2.7.1, 线性元求解泊松方程-混合边界问题
// void Exercise_271();

class Exercise_271
{
public:
    Exercise_271(double f_, double x1_, double x2_, double y1_, double y2_, double u_dx1, double u_dx2, unsigned xParts_);
    ~Exercise_271(){}
    bool IsInside(Point const &p_);
    std::vector<double> Get_abc(Point const &i_, Point const &j_, Point const &k_);

private:
    const double f;
    const std::pair<double, double> boundary_x;
    const std::pair<double, double> boundary_y;
    const std::pair<double, double> u_dx;
    const unsigned xParts;
    unsigned nodes;
    std::vector<unsigned> innerNodes;
    std::vector<unsigned> outerNodes;
    std::vector<Point> nodesData;
    double triArea;
};









#endif