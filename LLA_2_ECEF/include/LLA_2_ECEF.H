#ifndef __LLA_2_ECEF_H__
#define __LLA_2_ECEF_H__

#include <iostream>
// #include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"


#define PI 3.1415926535898
#define deg2rad PI/180.0
#define rad2deg 180.0/PI
#define WGS84_A 6378137.0
#define WGS84_f 1.0/298.257223563
#define WGS84_E2 WGS84_f*(2-WGS84_f)


class LLA_2_ECEF
{
public:
    LLA_2_ECEF(double, double, double );
    LLA_2_ECEF(double, double, double, double, double, double, double);
    ~LLA_2_ECEF();

    double lon;
    double lat;
    double alt;
    double X;
    double Y;
    double Z;

};


#endif