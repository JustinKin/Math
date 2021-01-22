#ifndef __SPHERECONSENSUS_H__
#define __SPHERECONSENSUS_H__

#include <iostream>
#include <Eigen/Eigen>

struct Point
{
    float x = 0, y = 0, z = 0;
};


struct SphereCoefficients
{
    std::vector<Point> pcd_in;
    Eigen::Matrix3f D, Dx, Dy, Dz;
    Eigen::Vector3f d;
    Eigen::Vector4f coe;
    Eigen::Matrix<float,4,3> D_in;

};

void ComputeSphere(std::string &filename_,SphereCoefficients &sphere_);

#endif