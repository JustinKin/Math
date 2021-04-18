#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "D:/Lib/Eigen3/Eigen/Eigen"
#include "D:/Lib/Eigen3/Eigen/LU"
#include "D:/Lib/FMT/include/fmt/format.h"
#include "D:/Lib/FMT/include/fmt/ranges.h"
#include "D:/Lib/FMT/include/fmt/os.h"

#include "NE4PED.H"

using namespace std;

/*============================================================
==  P82, 2.7.1, 线性元求解泊松方程-混合边界问题                   ==
==  求解区域: -1 < x, y < 1                                   ==
==  对区域均匀划分, 一共25个节点, 32个面积 s = 1/6 的等腰直角三角形  ==
==  其中, 未知节点15个, 已知节点10个                             ==
============================================================ */

// 显示网格参数
void Grid::ShowGridPara()
{
    // 网格剖分的参数
    double len = (boundary_x.second - boundary_x.first) / xParts;
    auto yParts = (boundary_y.second - boundary_y.first) / len;
    fmt::print("=== Initialized ===\n\n");
    fmt::print("    xParts : {}\n", xParts);
    fmt::print("    yParts : {}\n", yParts);
    fmt::print("     nodes : {}\n", nodes);
    fmt::print("outerNodes : {}\n", outerNodes.size());
    fmt::print("outerNodes No. : {}\n", outerNodes);
    fmt::print("innerNodes : {}\n", innerNodes.size());
    fmt::print("innerNodes No. : {}\n", innerNodes);
    // int count = 0;
    // for(auto const &c : nodesData)
    //     fmt::print("{}: ({}, {}, {})\n", count++, c.x, c.y, c.value);
    fmt::print("   triArea : {}\n", triArea);

}

Grid::Grid(double f_, double x1_, double x2_, double y1_, double y2_,unsigned xParts_)
        : f(f_), boundary_x(make_pair(x1_,x2_)), boundary_y(make_pair(y1_,y2_)), xParts(xParts_)
    {
        // 网格剖分的参数
        double len = (boundary_x.second - boundary_x.first) / xParts;
        auto yParts = (boundary_y.second - boundary_y.first) / len;
        nodes = (xParts + 1) * (yParts + 1);
        triArea = 0.5 * len * len;
        // 初始化节点坐标
        nodesData.resize(nodes);
        for(unsigned j =0, k = 0; j < xParts + 1; ++j)
        {
            for(unsigned i = 0; i < xParts + 1; ++i)
            {
                nodesData[k].x = boundary_x.first + len * j;
                nodesData[k++].y = boundary_y.second - len * i;
            }
        }

        unsigned scale = innerNodes.size();
        A.resize(scale,scale);
        b.resize(scale,1);

    }


bool Grid::IsInside(Point const &p_)
{
    return( p_.x >= boundary_x.first && p_.x <= boundary_x.second
         && p_.y >= boundary_y.first && p_.y <= boundary_y.second );
}


std::vector<double> Grid::Get_abc(Point const &i, Point const &j, Point const &k)
{
    double a = j.x * k.y - k.x * j.y;
    double b = j.y - k.y;
    double c = k.x - j.x;
    return {a,b,c};
}

// 线性方程组求解
void Grid::SolveEquation()
{
    auto u = A.lu().solve(b);
    for(unsigned i = 0; i < innerNodes.size(); ++i)
    {
        nodesData[innerNodes[i]].value = u(i,0);
    }
}


// 保存计算结果并画图
void Grid::GetResults(std::string fileName) const
{
    // 结果保存在result.txt中
    auto out = fmt::output_file("../result_" + fileName + ".txt");
    for(auto const &p : nodesData)
        out.print("( {: f}, {: f} ) : {: f}\n", p.x, p.y, p.value);
    out.close();
    // 画图
}

// 初始化内外节点编号
void Exercise_271::SetNodesNumb()
{
    unsigned outs = 2*(xParts + 1);
    unsigned ins = (xParts + 1)*(xParts + 1 ) - outs;
    outerNodes.resize(outs);
    innerNodes.resize(ins);
    for(unsigned i = 0; i < xParts + 1; ++i)
    {
        unsigned no = i * (xParts + 1);
        outerNodes[i*2] = no;
        outerNodes[i*2 + 1] = no + xParts;
    }
    for(unsigned j = 0, k = 0; j < xParts + 1; ++j)
    {
        for(unsigned i = 1; i < xParts; ++i)
            innerNodes[k++] = i + j * (xParts + 1);
    }
}


// 初始化外节点初值
void Exercise_271::SetNodesData()
{
    for(auto const &p : outerNodes)
        nodesData[p].value = u_e;
}


Exercise_271::
    Exercise_271(double f_, double x1_, double x2_, double y1_, double y2_,unsigned xParts_,
                 double u_e_, double u_dx1_, double u_dx2_)
        : Grid(f_, x1_, x2_, y1_, y2_, xParts_), u_e(u_e_), u_dx(make_pair(u_dx1_,u_dx2_))
    {
        // 网格剖分的参数
        double len = (boundary_x.second - boundary_x.first) / xParts;
        auto yParts = (boundary_y.second - boundary_y.first) / len;
        nodes = (xParts + 1) * (yParts + 1);
        triArea = 0.5 * len * len;
        // 初始化内外节点编号
        SetNodesNumb();
        // 初始化外节点初值
        SetNodesData();
        // 显示网格参数
        ShowGridPara();
    }


// 形成有限元方程
void Exercise_271::GeneratePDE()
{
    int scale = innerNodes.size();
    int edge1 = xParts-1;
    int edge2 = scale - edge1;
    // 右端第一项
    for(int j = 0; j < edge1; ++j)
        b(j,0) = 1.0 / 3.0 * triArea * f * 3;
    for(int j = edge1; j < edge2; ++j)
        b(j,0) = 1.0 / 3.0 * triArea * f * 6;
    for(int j = edge2; j < scale; ++j)
        b(j,0) = 1.0 / 3.0 * triArea * f * 3;

    // 右端第二项恒为0

    // 右端第三项

    // 方程左端
    // 对于左边界内点
    for(int j = 0; j < edge1; ++j) // j 是内点
    {
        int count = xParts + 2;
        auto const &p = nodesData;
        if(j == 0)
        {
            vector<double>abc(3);
            double b[6] = {0}, c[6] = {0};

            A(0,0);//3个三角形



            A(0,1);//1个三角形
            A(0,edge1);//2个三角形
            abc = Get_abc(p[xParts+2],p[0],p[1]);
            b[0] = abc[1];
            c[0] = abc[2];
            abc = Get_abc(p[xParts+1],p[0],p[xParts+2]);
            b[1] = abc[1];
            c[1] = abc[2];
            A(0,edge1) = 0.25 / triArea * (b[0] * b[1] + c[0] * c[1]);
        }

        for(int i = 0; i< count; ++i)
        {
            A(j,i);
            //  = 0 break;
            int iNumb = innerNodes[i];
            int jNumb = innerNodes[j];
            if(iNumb == jNumb)// 3个三角形
            {


            }
            else
            {
                // j 在上 邻接2点
                // j 在中间 邻接4点
                // j 在下 邻接3点
            }

        }
    }

    //对于中间内点
    for(int j = edge1; j < edge2; ++j) // j 是内点
    {
        for(int i = 0; i< scale; ++i)
        {
            A(j,i);
            //  = 0 break;
        }
    }

    // 对于右边界内点
    for(int j = edge2; j < scale; ++j) // j 是内点
    {
        for(int i = 0; i< scale; ++i)
        {
            A(j,i);
            //  = 0 break;
        }
    }

}