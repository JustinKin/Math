#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <random>
// #include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"
// #include "D:\QinJunyou\C\Eigen3\Eigen\LU"
#include "LLA_2_ECEF.H"

using namespace std;


LLA_2_ECEF::LLA_2_ECEF(double lo, double la, double al):
    lon(lo*deg2rad), lat(la*deg2rad), alt(al)
{
    double N = WGS84_A / (sqrt(1 - WGS84_E2 * sin(lat) * sin(lat)));
    X = (N + alt) * cos(lat) * cos(lon);
    Y = (N + alt) * cos(lat) * sin(lon);
    Z = (N * (1 - WGS84_f) * (1 - WGS84_f) + alt) * sin(lat);
}

LLA_2_ECEF::LLA_2_ECEF(double lod, double lom, double los,
                       double lad, double lam, double las, double al):
lon((((los/60.0)+lom)/60.0+lod)*deg2rad), lat((((las/60.0)+lam)/60.0+lad)*deg2rad), alt(al)
{
    double N = WGS84_A / (sqrt(1 - WGS84_E2 * sin(lat) * sin(lat)));
    X = (N + alt) * cos(lat) * cos(lon);
    Y = (N + alt) * cos(lat) * sin(lon);
    Z = (N * (1 - WGS84_f) * (1 - WGS84_f) + alt) * sin(lat);
}

LLA_2_ECEF::~LLA_2_ECEF() {};
