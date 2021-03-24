#ifndef __Optical_Axis_Transition_H__
#define __Optical_Axis_Transition_H__

#include <iostream>
#include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"



class Optical_Axis_Transition
{
    const double PI = 3.1415926535898;
    // 角度转弧度
    const double deg2rad = PI/180.0;
    // 弧度转角度
    const double rad2deg = 180.0/PI;
public:
    Optical_Axis_Transition(std::string unit_, double includedAngle_, Eigen::Matrix<double, 3,1> camVector_1_);
    ~Optical_Axis_Transition(){};
    // 获取结果, 结果的单位与输入一致
    Eigen::Matrix<double, 3,1> GetCam2Vector();
    // 解算
    void SolveCam2Vector(double rollAngle_);

private:
    // 统一单位
    double Unified_Unit(double includedAngle_);
    // 输入角度, 弧度 unit = "rad", 角度 unit = "deg"
    const std::string unit;
    // 两相机光轴夹角
    double includedAngle;
    // 相机1光轴方向向量
    const Eigen::Matrix<double, 3,1> camVector_1;
    // 相机2光轴方向向量
    Eigen::Matrix<double, 3,1> camVector_2;

};








#endif