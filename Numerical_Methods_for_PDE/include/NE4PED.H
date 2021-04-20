#ifndef __NE4PED_H__
#define __NE4PED_H__

#include <iostream>
#include <string>
#include <vector>
#include "D:/Lib/Eigen3/Eigen/Eigen"

class Point
{
public:
    double x;
    double y;
    double value;
    Point() : x(0), y(0), value(0) {}
    ~Point(){}
    Point(double x_, double y_) : x(x_), y(y_), value(0) {}
    Point(double x_, double y_, double value_) : x(x_), y(y_), value(value_) {}
    Point(const Point& p) : x(p.x), y(p.y), value(p.value) {}
};


//P82, 2.7.1, 线性元求解泊松方程-混合边界问题
class Exercise_271;
//P82, 2.7.2, 线性元求解Helmholtz方程-混合边界问题
class Exercise_272;

class Grid
{
friend class Exercise_271;
friend class Exercise_272;

public:
    Grid(double f_, double x1_, double x2_, double y1_, double y2_,unsigned xParts_);
    ~Grid(){}
    std::vector<double> Get_abc(Point const &i_, Point const &j_, Point const &k_);
    // 形成有限元方程
    virtual void GeneratePDE() = 0;
    // 线性方程组求解
    void SolveEquation(bool out_);
    // 保存计算结果并画图
    void GetResults(std::string fileName_) const;

private:
    const double f;
    const std::pair<double, double> boundary_x;
    const std::pair<double, double> boundary_y;
    const unsigned xParts;
    unsigned nodes;
    // 内节点编号
    std::vector<int> innerNodes;
    // 外节点编号
    std::vector<int> outerNodes;
    std::vector<Point> nodesData;
    double triArea;
    Eigen::MatrixXd A;
    Eigen::MatrixXd b;
    // 初始化内外节点编号
    virtual void SetNodesNumb() = 0;
    // 初始化外节点初值
    virtual void SetNodesData() = 0;
    // 显示网格参数
    void ShowGridPara();
};


class Exercise_271 : public Grid
{
public:
    Exercise_271(double f_, double x1_, double x2_, double y1_, double y2_,unsigned xParts_,
                 double u_e_, double u_dx1_, double u_dx2_);
    ~Exercise_271(){}
    void GeneratePDE() override;

private:
    //第一边值条件
    const double u_e;
    //第二边值条件
    const std::pair<double, double> u_dx;
    // 设置节点编号
    void SetNodesNumb() override;
    // 设置节点值
    void SetNodesData() override;
    // 处理左上left-top
    void ProcessLT();
    // 处理左上left-middle
    void ProcessLM();
    // 处理左下left-bottom
    void ProcessLB();
    // 处理右边界
    void ProcessR();

};

class Exercise_272 : public Grid
{
public:
    Exercise_272(double f_, double x1_, double x2_, double y1_, double y2_,unsigned xParts_,
                 double u_e_, double u_dx_, double k_);
    ~Exercise_272(){}
    void GeneratePDE() override;

private:
    //第一边值条件
    const double u_e;
    //第二边值条件
    const double u_dx;
    //k
    const double k;
    // 设置节点编号
    void SetNodesNumb() override;
    // 设置节点值
    void SetNodesData() override;
    // 处理左边
    void ProcessL();
    // 处理右边
    void ProcessR();

};

#endif