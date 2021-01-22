#ifndef __PLANECONSENSUS_H__
#define __PLANECONSENSUS_H__

#include <iostream>
#include <Eigen/Eigen>

struct Point
{
    float x = 0, y = 0, z = 0;
};

struct PlaneCoefficients
{
    std::vector<Point> pcd, pcd_in;
    Eigen::Matrix3f D, Dx, Dy, Dz;
    Eigen::Vector3f d;
    Eigen::Vector4f coe,coefficients;
    std::vector<Eigen::Vector4f> coe_o;
    Eigen::Matrix<float,3,3> D_in;
    double  iter = 0;
    int inliers = 0;
    float threshold = 0,threshold_dst = 0, threshold_iter = 0, probability = 0;

};

void Get3Points(PlaneCoefficients &plane_, int &i_);
void ComputePlane(std::string &filename_,PlaneCoefficients &plane_);
void Ransac(PlaneCoefficients &plane_);
void ShowResult(std::string &filename_,PlaneCoefficients &plane_);
bool Distance(Eigen::Matrix3f &D_,PlaneCoefficients &plane_);
void OptimizeModel(PlaneCoefficients &plane_);


#endif