#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <random>
#include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"
#include "D:\QinJunyou\C\Eigen3\Eigen\LU"
#include "NE4PED.H"

using namespace std;

/*============================================================
==  P82, 2.7.1, 线性元求解泊松方程-混合边界问题                   ==
==  求解区域: -1 < x, y < 1                                   ==
==  对区域均匀划分, 一共25个节点, 32个面积 s = 1/6 的等腰直角三角形  ==
==  其中, 未知节点15个, 已知节点10个                             ==
============================================================ */
// 计算a, b, c
vector<int> Get_abc(Triangle &t)
{
    int a = t.j.x * t.k.y - t.k.x * t.j.y;
    int b = t.j.y - t.k.y;
    int c = t.k.x - t.j.x;
    return {a,b,c};
}

void Exercise_271()
{
    const float f = 2.0;
    const float s = 1.0 / 8.0; //每个三角形面积
    const float beta = 1.0;
    Eigen::Matrix<float, 15, 15> A;
    Eigen::Matrix<float, 15, 1> b;

    // 初始化点的坐标
    vector<Point> v(15);
    int num = 0;
    for(int j = 0; j <5; ++j)
    {
        for(int i = 0; i <5; ++i)
        {
            v[num++].x = -1 + 0.5 * j;
            v[num++].y = 1 - 0.5 * i;
        }
    }
    vector<int> inner{1,2,3,6,7,8,11,12,13,16,17,18,21,22,23};  //内点编号
    vector<int> outer{0,4,5,9,10,14,15,19,20,24};  //外点编号

    num = 0;
    const float len = 0.5;
    for (int j = 0; j < 14; ++j)
    {
        auto it = find(outer.begin(), outer.end(), num);
        float aph = 1;
        if(it != outer.end())
            aph = 0;
        float tmp = 1.0 / 2.0 * s * 1.0 / 3.0 * f * aph;
        b(j, 0) = 1.0 / 2.0 * s * 1.0 / 3.0 * f - tmp + len;
        ++num;
    }

    num = 0;
    int l = 1, m = 2 , n = 6;
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 14; ++j)
        {

            Triangle t(v[l], v[m], v[n]);
            auto v = Get_abc(t);
            A(i, j) = 1.0 / 4 * 1 / s * (v[2] + v[3]);
            ++l;
            ++m;
            ++n;
        }
    }

    // 使用LU分解进行线性方程组求解
    Eigen::Matrix<float, 15, 1> u = A.lu().solve(b);
    cout << " 原方程的数值解为:\n " << u << endl;
}

