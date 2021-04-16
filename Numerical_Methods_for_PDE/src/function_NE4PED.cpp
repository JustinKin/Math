#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <random>
#include "D:\Lib\Eigen3\Eigen\Eigen"
#include "D:\Lib\Eigen3\Eigen\LU"
#include "D:\Lib\FMT\include\fmt\format.h"
#include "D:\Lib\FMT\include\fmt\ranges.h"
#include "D:\Lib\FMT\include\fmt\os.h"

#include "NE4PED.H"

using namespace std;

/*============================================================
==  P82, 2.7.1, 线性元求解泊松方程-混合边界问题                   ==
==  求解区域: -1 < x, y < 1                                   ==
==  对区域均匀划分, 一共25个节点, 32个面积 s = 1/6 的等腰直角三角形  ==
==  其中, 未知节点15个, 已知节点10个                             ==
============================================================ */
// 计算a, b, c
/* vector<int> Get_abc(Triangle &t)
{
    int a = t.j.x * t.k.y - t.k.x * t.j.y;
    int b = t.j.y - t.k.y;
    int c = t.k.x - t.j.x;
    return {a,b,c};
}
 */

Exercise_271::
    Exercise_271(double f_, double x1_, double x2_, double y1_, double y2_,
                 double u_dx1, double u_dx2, unsigned xParts_)
        : f(f_), boundary_x(make_pair(x1_,x2_)), boundary_y(make_pair(y1_,y2_)), u_dx(make_pair(u_dx1,u_dx2)),xParts(xParts_)
    {
        // 网格剖分的参数
        double len = (boundary_x.second - boundary_x.first) / xParts;
        auto yParts = (boundary_y.second - boundary_y.first) / len;
        nodes = (xParts + 1) * (yParts + 1);
        triArea = 0.5 * len * len;
        // 初始化内外点编号
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
        // 初始化内外点坐标
        nodesData.resize(nodes);
        for(unsigned j =0, k = 0; j < xParts + 1; ++j)
        {
            for(unsigned i = 0; i < xParts + 1; ++i)
            {
                nodesData[k].x = boundary_x.first + len * j;
                nodesData[k++].y = boundary_y.second - len * i;
            }
        }
        // 外点初值
        for(auto const &p : outerNodes)
            nodesData[p].value = 0;

        fmt::print("=== Initialized ===\n\n");
        fmt::print("    xParts : {}\n", xParts);
        fmt::print("    yParts : {}\n", yParts);
        fmt::print("     nodes : {}\n", nodes);
        fmt::print("outerNodes : {}\n", outs);
        fmt::print("outerNodes No. : {}\n", outerNodes);
        fmt::print("innerNodes : {}\n", ins);
        fmt::print("innerNodes No. : {}\n", innerNodes);
        int count = 0;
        // for(auto const &c : nodesData)
        //     fmt::print("{}: ({}, {}, {})\n", count++, c.x, c.y, c.value);
        fmt::print("   triArea : {}\n", triArea);
    }


bool Exercise_271::IsInside(Point const &p_)
{
    return( p_.x >= boundary_x.first && p_.x <= boundary_x.second
         && p_.y >= boundary_y.first && p_.y <= boundary_y.second );
}


std::vector<double> Exercise_271::
    Get_abc(Point const &i, Point const &j, Point const &k)
    {
        double a = j.x * k.y - k.x * j.y;
        double b = j.y - k.y;
        double c = k.x - j.x;
        return {a,b,c};
    }