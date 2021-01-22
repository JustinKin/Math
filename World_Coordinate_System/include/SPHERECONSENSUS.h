#ifndef __SPHERECONSENSUS_H__
#define __SPHERECONSENSUS_H__

#include <iostream>
#include <Eigen/Eigen>

/* struct Point
{
    float x = 0, y = 0, z = 0;
}; */

struct SphereCoefficients
{
    std::vector<Point> pcd, pcd_in;
    Eigen::Matrix3f D, Dx, Dy, Dz;
    Eigen::Vector3f d;
    Eigen::Vector4f coe,coefficients;
    std::vector<Eigen::Vector4f> coe_o;
    Eigen::Matrix<float,4,3> D_in;
    double  iter = 0;
    int inliers = 0;
    float threshold = 0,threshold_dst = 0, threshold_iter = 0, probability = 0;

};

void Get4Points(SphereCoefficients &sphere_, int &i_);
void ComputeSphere(std::string &filename_,SphereCoefficients &sphere_);
void Ransac(SphereCoefficients &sphere_);
// void ShowResult(std::string &filename_,SphereCoefficients &sphere_);
bool Distance(Eigen::Matrix4f &D_,SphereCoefficients &sphere_);
// void OptimizeModel(SphereCoefficients &sphere_);


#endif