#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "D:/Lib/Eigen3/Eigen/SVD"
#include "D:/Lib/FMT/include/fmt/format.h"
#include "D:/Lib/FMT/include/fmt/ranges.h"
#include "D:/Lib/FMT/include/fmt/os.h"
#include "D:/Lib/Matplotlib/matplotlibcpp.h"
#include "NE4PED.H"

using namespace std;
namespace plt = matplotlibcpp;

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
    // fmt::print("outerNodes No. : {}\n", outerNodes);
    fmt::print("innerNodes : {}\n", innerNodes.size());
    // fmt::print("innerNodes No. : {}\n", innerNodes);
    fmt::print("   triArea : {}\n", triArea);
}

Grid::Grid(double f_, double x1_, double x2_, double y1_, double y2_, unsigned xParts_)
    : f(f_), boundary_x(make_pair(x1_, x2_)), boundary_y(make_pair(y1_, y2_)), xParts(xParts_)
{
    // 网格剖分的参数
    double len = (boundary_x.second - boundary_x.first) / xParts;
    auto yParts = (boundary_y.second - boundary_y.first) / len;
    nodes = (xParts + 1) * (yParts + 1);
    triArea = 0.5 * len * len;
    // 初始化节点坐标
    nodesData.resize(nodes);
    for (unsigned j = 0, k = 0; j < xParts + 1; ++j)
    {
        for (unsigned i = 0; i < xParts + 1; ++i)
        {
            nodesData[k].x = boundary_x.first + len * j;
            nodesData[k++].y = boundary_y.second - len * i;
        }
    }
}

std::vector<double> Grid::Get_abc(Point const &i, Point const &j, Point const &k)
{
    double a = j.x * k.y - k.x * j.y;
    double b = j.y - k.y;
    double c = k.x - j.x;
    return {a, b, c};
}

// 线性方程组求解
void Grid::SolveEquation(bool out)
{
    int rows = innerNodes.size();
    Eigen::MatrixXd u = Eigen::MatrixXd::Zero(rows, 1);
    u = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
    if (out)
    {
        cout << "\n"
             << A << endl;
        cout << "\n"
             << b << endl;
        cout << "\n"
             << u << endl;
    }

    for (unsigned i = 0; i < innerNodes.size(); ++i)
    {
        nodesData[innerNodes[i]].value = u(i, 0);
    }
}

// 保存计算结果并画图
void Grid::GetResults(std::string fileName) const
{
    // 结果保存在result.txt中
    auto out = fmt::output_file("../Numerical_Methods_for_PDE/result_" + fileName + ".txt");
    for (auto const &p : nodesData)
        out.print("{: f},{: f},{: f}\n", p.x, p.y, p.value);
    // out.print("( {: f}, {: f} ) : {: f}\n", p.x, p.y, p.value);
    out.close();
    // 画图
    std::vector<std::vector<double>> x, y, z;
    for (int i = 0; i < nodesData.size();)
    {
        std::vector<double> x_row, y_row, z_row;
        for (int j = 0; j < xParts + 1; ++i, ++j)
        {
            x_row.emplace_back(nodesData[i].x);
            y_row.emplace_back(nodesData[i].y);
            z_row.emplace_back(nodesData[i].value);
        }
        x.emplace_back(x_row);
        y.emplace_back(y_row);
        z.emplace_back(z_row);
    }
    plt::plot_surface(x, y, z);
    plt::show();
}


// 初始化内外节点编号
void Exercise_271::SetNodesNumb()
{
    unsigned outs = 2 * (xParts + 1);
    unsigned ins = (xParts + 1) * (xParts + 1) - outs;
    outerNodes.resize(outs);
    innerNodes.resize(ins);
    for (unsigned i = 0; i < xParts + 1; ++i)
    {
        unsigned no = i * (xParts + 1);
        outerNodes[i * 2] = no;
        outerNodes[i * 2 + 1] = no + xParts;
    }
    for (unsigned j = 0, k = 0; j < xParts + 1; ++j)
    {
        for (unsigned i = 1; i < xParts; ++i)
            innerNodes[k++] = i + j * (xParts + 1);
    }
}

// 初始化外节点初值
void Exercise_271::SetNodesData()
{
    for (auto const &p : outerNodes)
        nodesData[p].value = u_e;
}

Exercise_271::
    Exercise_271(double f_, double x1_, double x2_, double y1_, double y2_, unsigned xParts_,
                 double u_e_, double u_dx1_, double u_dx2_)
    : Grid(f_, x1_, x2_, y1_, y2_, xParts_), u_e(u_e_), u_dx(make_pair(u_dx1_, u_dx2_))
{
    unsigned scale = (xParts + 1) * (xParts + 1) - 2 * (xParts + 1);
    A = Eigen::MatrixXd::Zero(scale, scale);
    b = Eigen::MatrixXd::Zero(scale, 1);

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

// 处理左上left-top
void Exercise_271::ProcessLT()
{
    const auto &p = nodesData;
    vector<vector<double>> v(3);
    v[0] = Get_abc(p[1], p[xParts + 1], p[0]);
    v[1] = Get_abc(p[1], p[xParts + 2], p[xParts + 1]);
    v[2] = Get_abc(p[1], p[2], p[xParts + 2]);
    //j = 0, i = 0, 3个三角形
    A(0, 0) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
    //j = 0, i = 1, 1个三角形
    A(0, 1) = 0.25 / triArea * (v[2][1] + v[2][2]);
    //j = 0, i = 4, 2个三角形
    A(0, xParts - 1) = 0.25 / triArea * (v[1][1] * v[2][1] + v[1][2] * v[2][2]);
}

// 处理左上left-middle
void Exercise_271::ProcessLM()
{
    const int edge1 = xParts - 1;
    for (int j = 1; j < edge1 - 1; ++j) // j表示行
    {
        const auto &p = nodesData;
        vector<vector<double>> v(3);
        v[0] = Get_abc(p[j + 1], p[j + 1 + xParts], p[j]);
        v[1] = Get_abc(p[j + 1], p[j + 2 + xParts], p[j + 1 + xParts]);
        v[2] = Get_abc(p[j + 1], p[j + 2], p[j + 2 + xParts]);
        //5种情况: 每行5项
        //#1 1个三角形
        A(j, j - 1) = 0.25 / triArea * (v[0][1] + v[0][2]);
        //#2 3个三角形
        A(j, j) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
        //#3 1个三角形
        A(j, j + 1) = 0.25 / triArea * (v[2][1] + v[2][2]);
        //#4 2个三角形
        A(j, j + xParts - 2) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
        //#5 2个三角形
        A(j, j + xParts - 1) = 0.25 / triArea * (v[1][1] * v[2][1] + v[1][2] * v[2][2]);
    }
}

// 处理左下left-bottom
void Exercise_271::ProcessLB()
{
    const int row = xParts - 2;
    const auto &p = nodesData;
    vector<vector<double>> v(3);
    v[0] = Get_abc(p[row + 1], p[2 * xParts - 1], p[row]);
    v[1] = Get_abc(p[row + 1], p[2 * xParts], p[2 * xParts - 1]);
    v[2] = Get_abc(p[row + 1], p[xParts], p[2 * xParts]);
    // i = 3,4,9,10
    //i = 3, 1个三角形
    A(row, row - 1) = 0.25 / triArea * (v[0][1] + v[0][2]);
    //i = 4, 3个三角形
    A(row, row) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
    //i = 9, 2个三角形
    A(row, row + xParts - 2) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
    //i = 10, 2个三角形
    A(row, row + xParts - 1) = 0.25 / triArea * (v[1][1] * v[2][1] + v[1][2] * v[2][2]);
}

void Exercise_271::ProcessR()
{
    // 右上
    const auto &p = nodesData;
    vector<vector<double>> v(3);
    const int edge1 = xParts - 1;
    int row = innerNodes.size() - xParts + 1; // 矩阵行下标
    const int top = nodes - xParts;           //点号 不是内点矩阵下标
    const int bottom = nodes - 2;             //点号 不是内点矩阵下标
    // 处理右上right-top
    v[0] = Get_abc(p[top], p[top - 1], p[top - 1 - xParts]);
    v[1] = Get_abc(p[top], p[top - 1 - xParts], p[top - xParts]);
    v[2] = Get_abc(p[top], p[top - xParts], p[top + 1]);
    // 4个点 4项
    A(row, row - edge1) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
    A(row, row - edge1 + 1) = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
    A(row, row) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
    A(row, row + 1) = 0.25 / triArea * (v[2][1] + v[2][2]);
    ++row;

    //处理右中right-middle
    for (int j = top + 1; j < bottom; ++j) // j 是点号 不是内点矩阵下标
    {
        v[0] = Get_abc(p[j], p[j - 1], p[j - 1 - xParts]);
        v[1] = Get_abc(p[j], p[j - 1 - xParts], p[j - xParts]);
        v[2] = Get_abc(p[j], p[j - xParts], p[j + 1]);
        // 5个点 5项
        A(row, row - edge1) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
        A(row, row - edge1 + 1) = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
        A(row, row) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
        A(row, row + 1) = 0.25 / triArea * (v[2][1] + v[2][2]);
        A(row, row - 1) = 0.25 / triArea * (v[0][1] + v[0][2]);
        ++row;
    }

    // 处理右下right-bottom
    v[0] = Get_abc(p[bottom], p[bottom - 1], p[bottom - 1 - xParts]);
    v[1] = Get_abc(p[bottom], p[bottom - 1 - xParts], p[bottom - xParts]);
    v[2] = Get_abc(p[bottom], p[bottom - xParts], p[bottom + 1]);
    // 3个点 3项
    A(row, row - edge1) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
    A(row, row) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
    A(row, row - 1) = 0.25 / triArea * (v[0][1] + v[0][2]);
}

// 形成有限元方程
void Exercise_271::GeneratePDE()
{
    const int scale = innerNodes.size();
    const int edge1 = xParts - 1;
    const int edge2 = scale - edge1;
    // 方程右端
    // 右端第一项和第三项
    const auto &beta1 = u_dx.first;  // 1
    const auto &beta2 = u_dx.second; // 0
    for (int j = 0; j < edge1; ++j)
    {
        b(j, 0) = 1.0 / 3.0 * triArea * f * 3;
        b(j, 0) += beta1 * 2 * triArea;
    }
    for (int j = edge1; j < edge2; ++j)
        b(j, 0) = 1.0 / 3.0 * triArea * f * 6;
    for (int j = edge2; j < scale; ++j)
    {
        b(j, 0) = 1.0 / 3.0 * triArea * f * 3;
        b(j, 0) += beta2 * 2 * triArea;
    }
    // 右端第二项恒为0

    // 方程左端
    //处理左上left-top
    ProcessLT();
    // 处理左边界中间
    ProcessLM();
    // 处理左下left-bottom
    ProcessLB();

    //对于中间内点
    int row = edge1; // 矩阵行下标
    for (int col = 1; col < xParts; ++col)
    {
        const auto &p = nodesData;
        vector<vector<double>> v(6);
        const int top = 1 + col * (xParts + 1); //点号 不是内点矩阵下标
        v[0] = Get_abc(p[top], p[top - 1], p[top - 1 - xParts]);
        v[1] = Get_abc(p[top], p[top - 1 - xParts], p[top - xParts]);
        v[2] = Get_abc(p[top], p[top - xParts], p[top + 1]);
        v[3] = Get_abc(p[top], p[top + 1], p[top + 1 + xParts]);
        v[4] = Get_abc(p[top], p[top + 1 + xParts], p[top + xParts]);
        v[5] = Get_abc(p[top], p[top + xParts], p[top - 1]);
        // 5个点 5项
        A(row, row - edge1) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
        A(row, row - edge1 + 1) = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
        A(row, row) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] * v[3][1] * v[4][1] * v[5][1] + v[0][2] * v[1][2] * v[2][2] * v[3][2] * v[4][2] * v[5][2]);
        A(row, row + 1) = 0.25 / triArea * (v[2][1] * v[3][1] + v[2][2] * v[3][2]);
        A(row, row + edge1) = 0.25 / triArea * (v[4][1] * v[3][1] + v[4][2] * v[3][2]);
        ++row;
        const int bottom = (col + 1) * (xParts + 1) - 2; // 点号 不是内点矩阵下标
        for (int j = top + 1; j < bottom; ++j) // j 是点号 不是内点矩阵下标
        {
            v[0] = Get_abc(p[j], p[j - 1], p[j - 1 - xParts]);
            v[1] = Get_abc(p[j], p[j - 1 - xParts], p[j - xParts]);
            v[2] = Get_abc(p[j], p[j - xParts], p[j + 1]);
            v[3] = Get_abc(p[j], p[j + 1], p[j + 1 + xParts]);
            v[4] = Get_abc(p[j], p[j + 1 + xParts], p[j + xParts]);
            v[5] = Get_abc(p[j], p[j + xParts], p[j - 1]);
            // 7个点 7项
            A(row, row - edge1) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
            A(row, row - edge1 + 1) = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
            A(row, row) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] * v[3][1] * v[4][1] * v[5][1] + v[0][2] * v[1][2] * v[2][2] * v[3][2] * v[4][2] * v[5][2]);
            A(row, row + 1) = 0.25 / triArea * (v[2][1] * v[3][1] + v[2][2] * v[3][2]);
            A(row, row + edge1) = 0.25 / triArea * (v[4][1] * v[3][1] + v[4][2] * v[3][2]);
            A(row, row + edge1 - 1) = 0.25 / triArea * (v[4][1] * v[5][1] + v[4][2] * v[5][2]);
            A(row, row - 1) = 0.25 / triArea * (v[0][1] * v[5][1] + v[0][2] * v[5][2]);
            ++row;
        }
        // 5个点 5项
        v[0] = Get_abc(p[bottom], p[bottom - 1], p[bottom - 1 - xParts]);
        v[1] = Get_abc(p[bottom], p[bottom - 1 - xParts], p[bottom - xParts]);
        v[2] = Get_abc(p[bottom], p[bottom - xParts], p[bottom + 1]);
        v[3] = Get_abc(p[bottom], p[bottom + 1], p[bottom + 1 + xParts]);
        v[4] = Get_abc(p[bottom], p[bottom + 1 + xParts], p[bottom + xParts]);
        v[5] = Get_abc(p[bottom], p[bottom + xParts], p[bottom - 1]);
        A(row, row - edge1) = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
        A(row, row) = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] * v[3][1] * v[4][1] * v[5][1] + v[0][2] * v[1][2] * v[2][2] * v[3][2] * v[4][2] * v[5][2]);
        A(row, row + edge1) = 0.25 / triArea * (v[4][1] * v[3][1] + v[4][2] * v[3][2]);
        A(row, row + edge1 - 1) = 0.25 / triArea * (v[4][1] * v[5][1] + v[4][2] * v[5][2]);
        A(row, row - 1) = 0.25 / triArea * (v[0][1] * v[5][1] + v[0][2] * v[5][2]);
        ++row;
    }

    // 处理右边界
    ProcessR();
}


// 初始化内外节点编号
void Exercise_272::SetNodesNumb()
{
    unsigned outs = 2 * xParts + 1;
    unsigned ins = (xParts + 1) * (xParts + 1) - outs;
    outerNodes.resize(outs);
    innerNodes.resize(ins);
    outerNodes[0] = 0;
    for (unsigned i = 1; i < xParts + 1; ++i)
    {
        unsigned no = i * (xParts + 1);
        outerNodes[i] = i;
        outerNodes[xParts + i] = no;
    }
    for (unsigned j = 0, k = 0; j < xParts + 1; ++j)
    {
        for (unsigned i = 1; i < xParts + 1; ++i)
            innerNodes[k++] = i + (j + 1) * (xParts + 1);
    }
}

// 初始化外节点初值
void Exercise_272::SetNodesData()
{
    for (auto const &p : outerNodes)
        nodesData[p].value = u_e;
}

// 处理左边
void Exercise_272::ProcessL()
{
    const auto &p = nodesData;
    vector<vector<double>> v(6);
    const int top = xParts + 2; //点号 不是内点矩阵下标
    v[0] = Get_abc(p[top], p[top - 1], p[top - 1 - xParts]);
    v[1] = Get_abc(p[top], p[top - 1 - xParts], p[top - xParts]);
    v[2] = Get_abc(p[top], p[top - xParts], p[top + 1]);
    v[3] = Get_abc(p[top], p[top + 1], p[top + 1 + xParts]);
    v[4] = Get_abc(p[top], p[top + 1 + xParts], p[top + xParts]);
    v[5] = Get_abc(p[top], p[top + xParts], p[top - 1]);

    double sum = 0.0;
    // 0号点有3项
    sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] * v[3][1] * v[4][1] * v[5][1] + v[0][2] * v[1][2] * v[2][2] * v[3][2] * v[4][2] * v[5][2]);
    A(0, 0) = sum - 1.0 / 12.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[2][1] * v[3][1] + v[2][2] * v[3][2]);
    A(0, 1) = sum - 1.0 / 6.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[4][1] * v[3][1] + v[4][2] * v[3][2]);
    A(0, xParts) = sum - 1.0 / 6.0 * k * k * triArea;

    // 中间点有5项
    for (int j = 1; j < xParts - 1; ++j)
    {
        int middle = top + j;
        v[0] = Get_abc(p[middle], p[middle - 1], p[middle - 1 - xParts]);
        v[1] = Get_abc(p[middle], p[middle - 1 - xParts], p[middle - xParts]);
        v[2] = Get_abc(p[middle], p[middle - xParts], p[middle + 1]);
        v[3] = Get_abc(p[middle], p[middle + 1], p[middle + 1 + xParts]);
        v[4] = Get_abc(p[middle], p[middle + 1 + xParts], p[middle + xParts]);
        v[5] = Get_abc(p[middle], p[middle + xParts], p[middle - 1]);
        sum = 0.25 / triArea * (v[0][1] * v[5][1] + v[0][2] * v[5][2]);
        A(j, j - 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] * v[3][1] * v[4][1] * v[5][1] + v[0][2] * v[1][2] * v[2][2] * v[3][2] * v[4][2] * v[5][2]);
        A(j, j) = sum - 1.0 / 12.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[2][1] * v[3][1] + v[2][2] * v[3][2]);
        A(j, j + 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[4][1] * v[5][1] + v[4][2] * v[5][2]);
        A(j, j + xParts - 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[4][1] * v[3][1] + v[4][2] * v[3][2]);
        A(j, j + xParts) = sum - 1.0 / 6.0 * k * k * triArea;
    }

    // 底下点有4项
    int bottom = 2 * xParts + 1;
    const int row = xParts - 1;
    v[0] = Get_abc(p[bottom], p[bottom - 1], p[xParts]);
    v[1] = Get_abc(p[bottom], p[bottom + xParts], p[bottom - 1]);
    v[2] = Get_abc(p[bottom], p[bottom + xParts + 1], p[bottom + xParts]);
    sum = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
    A(row, row - 1) = sum - 1.0 / 6.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
    A(row, row) = sum - 1.0 / 12.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[1][1] * v[2][1] + v[1][2] * v[2][2]);
    A(row, row + xParts - 1) = sum - 1.0 / 6.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[2][1] + v[2][2]);
    A(row, row + xParts) = sum - 1.0 / 6.0 * k * k * triArea;
}

// 处理右边
void Exercise_272::ProcessR()
{
    // 右上
    const auto &p = nodesData;
    vector<vector<double>> v(3);
    int row = innerNodes.size() - xParts; // 矩阵行下标
    const int top = nodes - xParts;       //点号 不是内点矩阵下标
    const int bottom = nodes - 1;         //点号 不是内点矩阵下标
    v[0] = Get_abc(p[top], p[top - 1], p[top - 1 - xParts]);
    v[1] = Get_abc(p[top], p[top - 1 - xParts], p[top - xParts]);
    v[2] = Get_abc(p[top], p[top - xParts], p[top + 1]);
    double sum = 0.0;
    // 4个点 4项
    sum = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
    A(row, row - xParts) = sum - 1.0 / 6.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
    A(row, row - xParts + 1) = sum - 1.0 / 6.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
    A(row, row) = sum - 1.0 / 12.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[2][1] + v[2][2]);
    A(row, row + 1) = sum - 1.0 / 6.0 * k * k * triArea;
    ++row;

    // 中间点
    int middle = top + 1;
    for (int j = 1; j < xParts - 1; ++j)
    {
        v[0] = Get_abc(p[middle], p[middle - 1], p[middle - 1 - xParts]);
        v[1] = Get_abc(p[middle], p[middle - 1 - xParts], p[middle - xParts]);
        v[2] = Get_abc(p[middle], p[middle - xParts], p[middle + 1]);
        // 5个点 5项
        sum = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
        A(row, row - xParts) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
        A(row, row - xParts + 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
        A(row, row) = sum - 1.0 / 12.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[2][1] + v[2][2]);
        A(row, row + 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[0][1] + v[0][2]);
        A(row, row - 1) = sum - 1.0 / 6.0 * k * k * triArea;
        ++row;
        ++middle;
    }

    // 处理底边点
    // 3个点 3项
    v[0] = Get_abc(p[middle], p[middle - 1], p[middle - 1 - xParts]);
    sum = 0.25 / triArea * (v[0][1] + v[0][2]);
    A(row, row - xParts) = sum - 1.0 / 6.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[0][1] + v[0][2]);
    A(row, row) = sum - 1.0 / 12.0 * k * k * triArea;
    sum = 0.25 / triArea * (v[0][1] + v[0][2]);
    A(row, row - 1) = sum - 1.0 / 6.0 * k * k * triArea;
}

Exercise_272::
    Exercise_272(double f_, double x1_, double x2_, double y1_, double y2_, unsigned xParts_,
                 double u_e_, double u_dx_, int k_)
    : Grid(f_, x1_, x2_, y1_, y2_, xParts_), u_e(u_e_), u_dx(u_dx_), k(k_)
{
    unsigned scale = xParts * xParts;
    A = Eigen::MatrixXd::Zero(scale, scale);
    b = Eigen::MatrixXd::Zero(scale, 1); // 网格剖分的参数
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

void Exercise_272::GeneratePDE()
{
    const int scale = innerNodes.size();
    const int edge1 = xParts - 1;
    const int edge2 = scale - edge1;

    const int &rows = xParts; //4
    const int &cols = xParts; //4
    // 方程右端
    int edgeRT = 0;
    for (int col = 1, &j = edgeRT; col < cols; ++col)
    {
        for (int row = 1; row < rows; ++row)
        {
            b(j, 0) = 1.0 / 3.0 * triArea * f * 6;
            ++j;
        }
        b(j, 0) = 1.0 / 3.0 * triArea * f * 3;
        ++j;
    }
    for (int row = 1, &j = edgeRT; row < rows; ++row)
    {
        b(j, 0) = 1.0 / 3.0 * triArea * f * 3;
        ++j;
    }
    b(innerNodes.size() - 1, 0) = 1.0 / 3.0 * triArea * f;

    // 方程左端
    // 处理左边
    ProcessL();

    // 处理中部
    int row = xParts; // 矩阵行下标
    for (int col = 2; col < xParts; ++col)
    {
        const auto &p = nodesData;
        vector<vector<double>> v(6);
        // 处理顶上点
        const int top = 1 + col * (xParts + 1); //点号 不是内点矩阵下标
        v[0] = Get_abc(p[top], p[top - 1], p[top - 1 - xParts]);
        v[1] = Get_abc(p[top], p[top - 1 - xParts], p[top - xParts]);
        v[2] = Get_abc(p[top], p[top - xParts], p[top + 1]);
        v[3] = Get_abc(p[top], p[top + 1], p[top + 1 + xParts]);
        v[4] = Get_abc(p[top], p[top + 1 + xParts], p[top + xParts]);
        v[5] = Get_abc(p[top], p[top + xParts], p[top - 1]);
        double sum = 0.0;
        // 5个点 5项
        sum = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
        A(row, row - xParts) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
        A(row, row - xParts + 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] * v[3][1] * v[4][1] * v[5][1] + v[0][2] * v[1][2] * v[2][2] * v[3][2] * v[4][2] * v[5][2]);
        A(row, row) = sum - 1.0 / 12.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[2][1] * v[3][1] + v[2][2] * v[3][2]);
        A(row, row + 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[4][1] * v[3][1] + v[4][2] * v[3][2]);
        A(row, row + xParts) = sum - 1.0 / 6.0 * k * k * triArea;
        ++row;
        // 处理中间点
        const int bottom = (col + 1) * (xParts + 1) - 1; // 点号 不是内点矩阵下标
        for (int j = top + 1; j < bottom; ++j)           // j 是点号 不是内点矩阵下标
        {
            v[0] = Get_abc(p[j], p[j - 1], p[j - 1 - xParts]);
            v[1] = Get_abc(p[j], p[j - 1 - xParts], p[j - xParts]);
            v[2] = Get_abc(p[j], p[j - xParts], p[j + 1]);
            v[3] = Get_abc(p[j], p[j + 1], p[j + 1 + xParts]);
            v[4] = Get_abc(p[j], p[j + 1 + xParts], p[j + xParts]);
            v[5] = Get_abc(p[j], p[j + xParts], p[j - 1]);
            // 7个点 7项
            sum = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
            A(row, row - xParts) = sum - 1.0 / 6.0 * k * k * triArea;
            sum = 0.25 / triArea * (v[2][1] * v[1][1] + v[2][2] * v[1][2]);
            A(row, row - xParts + 1) = sum - 1.0 / 6.0 * k * k * triArea;
            sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] * v[3][1] * v[4][1] * v[5][1] + v[0][2] * v[1][2] * v[2][2] * v[3][2] * v[4][2] * v[5][2]);
            A(row, row) = sum - 1.0 / 12.0 * k * k * triArea;
            sum = 0.25 / triArea * (v[2][1] * v[3][1] + v[2][2] * v[3][2]);
            A(row, row + 1) = sum - 1.0 / 6.0 * k * k * triArea;
            sum = 0.25 / triArea * (v[4][1] * v[3][1] + v[4][2] * v[3][2]);
            A(row, row + xParts) = sum - 1.0 / 6.0 * k * k * triArea;
            sum = 0.25 / triArea * (v[4][1] * v[5][1] + v[4][2] * v[5][2]);
            A(row, row + xParts - 1) = sum - 1.0 / 6.0 * k * k * triArea;
            sum = 0.25 / triArea * (v[0][1] * v[5][1] + v[0][2] * v[5][2]);
            A(row, row - 1) = sum - 1.0 / 6.0 * k * k * triArea;
            ++row;
        }
        // 处理底边点
        // 5个点 5项
        v[0] = Get_abc(p[bottom], p[bottom - 1], p[bottom - 1 - xParts]);
        v[1] = Get_abc(p[bottom], p[bottom + xParts], p[bottom - 1]);
        v[2] = Get_abc(p[bottom], p[bottom + xParts + 1], p[bottom + xParts]);
        sum = 0.25 / triArea * (v[0][1] + v[0][2]);
        A(row, row - xParts) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[0][1] * v[1][1] + v[0][2] * v[1][2]);
        A(row, row - 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[0][1] * v[1][1] * v[2][1] + v[0][2] * v[1][2] * v[2][2]);
        A(row, row) = sum - 1.0 / 12.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[1][1] * v[2][1] + v[1][2] * v[2][2]);
        A(row, row + xParts - 1) = sum - 1.0 / 6.0 * k * k * triArea;
        sum = 0.25 / triArea * (v[2][1] + v[2][2]);
        A(row, row + xParts) = sum - 1.0 / 6.0 * k * k * triArea;
        ++row;
    }

    // 处理右边
    ProcessR();
}
