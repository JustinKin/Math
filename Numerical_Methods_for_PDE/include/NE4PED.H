#ifndef __NE4PED_H__
#define __NE4PED_H__

#include <iostream>
#include <memory>
#include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"

class Point
{
public:
    float x;
    float y;
    // Point* next;
    std::shared_ptr<Point> next;
    Point() : x(0), y(0), next(nullptr){}
    Point(const Point& p) : x(p.x), y(p.y), next(nullptr){}
    ~Point(){}
};

class Triangle
{
public:
    Point i;
    Point j;
    Point k;
    Triangle(Point i_, Point j_, Point k_) : i(i_), j(j_), k(k_)
    {
        // i.next = &j;
        i.next =std::make_shared<Point>(j);
        // j.next = &k;
        // k.next = &i;
    }
    ~Triangle(){}
};

//P82, 2.7.1, 线性元求解泊松方程-混合边界问题
void Exercise_271();











#endif