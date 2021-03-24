#include <iostream>
#include <cmath>
#include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"
#include "Optical_Axis_Transition.H"

using namespace std;

Optical_Axis_Transition::
    Optical_Axis_Transition(std::string unit_, double includedAngle_, Eigen::Matrix<double, 3,1> camVector_1_) :
        unit(unit_), camVector_1(camVector_1_)
    {
        includedAngle = Unified_Unit(includedAngle_);
    }


double Optical_Axis_Transition::
    Unified_Unit(double includedAngle_)
    {
        return ( unit == "deg" ?  (includedAngle_ * deg2rad) : (includedAngle_));
    }

Eigen::Matrix<double, 3,1> Optical_Axis_Transition::
    GetCam2Vector()
    {
        return camVector_2;
    }


void Optical_Axis_Transition::
    SolveCam2Vector(double rollAngle_)
    {
        const auto v1 = camVector_1;
        const auto l1 = camVector_1(0,0);
        const auto m1 = camVector_1(1,0);
        const auto n1 = camVector_1(2,0);
        const double alpha1 = atan(m1 / (sqrt( (l1 * l1) + (n1 * n1) )));
        const double l2 = -l1;
        const double n2 = -n1;
        const double alpha2 = -(PI - includedAngle + alpha1);
        const double m2 = sqrt( l2*l2 + n2*n2 ) * tan(alpha2);
        Eigen::Matrix<double, 3,1> v2;
        v2 << l2 , m2, n2;
        auto v2_norm = v2 / (v2.norm());

        Eigen::Matrix<double, 3,3> nx;
        nx << 0, -n1,  m1,
              n1,  0, -l1,
             -m1, l1,   0;
        auto I = Eigen::Matrix3d::Identity();
        double rollAngle = Unified_Unit(rollAngle_);
        auto R = ( cos(rollAngle) * I + (1-cos(rollAngle)) * v1 * v1.transpose() - sin(rollAngle) * nx).transpose();
        camVector_2 = R * v2_norm;
    }
