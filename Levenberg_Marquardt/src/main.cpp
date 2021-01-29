/*
                           _ooOoo_
                          o8888888o
                          88" . "88
                          (| -_- |)
                          O\  =  /O
                       ____/`---'\____
                     .'  \\|     |//  `.
                    /  \\|||  :  |||//  \
                   /  _||||| -:- |||||-  \
                   |   | \\\  -  /// |   |
                   | \_|  ''\---/''  |   |
                   \  .-\__  `-`  ___/-. /
                 ___`. .'  /--.--\  `. . __
              ."" '<  `.___\_<|>_/___.'  >'"".
             | | :  `- \`.;`\ _ /`;.`/ - ` : | |
             \  \ `-.   \_ __\ /__ _/   .-` /  /
        ======`-.____`-.___\_____/___.-`____.-'======
                           `=---='
        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
          一心向善	好运相伴	佛祖保佑	永无BUG
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <memory>
#include <cmath>
#include "D:\FMT\include\fmt\format.h"
#include "D:\FMT\include\fmt\ranges.h"
#include "D:\FMT\include\fmt\os.h"
#include "LM.H"

using namespace std;

int main(int argc, char **argv)
{
    std::ios::sync_with_stdio(false);

    auto ov = Test();
    // vector<std::pair<double, double>> ov{{1,5},{-1,1}};
	for(const auto &c : ov)
	{
		fmt::print("{: f}, {: f}\n", c.first[0], c.second);
	}
    vector<double> iv{0,0,0};
    // vector<double> iv{1,1};
    // 提供待求量的迭代初始值
    vector<double> iv1{0,0};

    //观测值类型为 vector<pair<vector<double>, double>>
    // 由观测向量(值)和结果成对组成的vector
    vector<double> v1{3,1},v2{1,2},v3{6,8};
    vector<pair<vector<double>, double>> ov1{make_pair(v1,7),make_pair(v2,5),make_pair(v3,25.8156)};
    // ov1.emplace_back(make_pair(v1,7));
    // ov1.emplace_back(make_pair(v2,5));

    // 提供原方程组,传入观测向量(值)和待求量
    // auto fx1 = [](const vector<double> x,const vector<double> v) -> double{return exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};
    auto fx1 = [](const vector<double> x,const vector<double> v) -> double{return x[0] * v[0] * v[0] + x[1] * v[1];};
    auto fx2 = [](const vector<double> x,const vector<double> v) -> double{return x[0] * v[0] + x[1] * v[1] * v[1];};
    auto fx3 = [](const vector<double> x,const vector<double> v) -> double{return x[0] * v[0] * v[0] + x[1] * v[1] * v[1];};

    // 提供雅可比矩阵生成函数，注意符号为负,传入观测向量(值)和待求量
    // auto jx1 = [](const vector<double> x,const vector<double> v) -> double{return -x[0]*x[0] * exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};
    // auto jx2 = [](const vector<double> x,const vector<double> v) -> double{return -x[0] * exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};
    // auto jx3 = [](const vector<double> x,const vector<double> v) -> double{return -exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};
    auto jx1 = [](const vector<double> x,const vector<double> v) -> double{return -(6*v[0]);};
    auto jx2 = [](const vector<double> x,const vector<double> v) -> double{return -1;};
    auto jx3 = [](const vector<double> x,const vector<double> v) -> double{return -1;};
    auto jx4 = [](const vector<double> x,const vector<double> v) -> double{return -(4*v[1]);};
    auto jx5 = [](const vector<double> x,const vector<double> v) -> double{return -(12*v[0]);};
    auto jx6 = [](const vector<double> x,const vector<double> v) -> double{return -(16*v[1]);};

    // auto f0 = [](const double x,const vector<double> v) -> double{return v[0] * x + v[1];};
    // auto f1 = [](const double x,const vector<double> v) -> double{return -x;};
    // auto f2 = [](const double x,const vector<double> v) -> double{return -1;};

    //传入观测向量(值)和待求量的函数指针
    using funPtr = double (*)(vector<double>,vector<double>);
    // vector<funPtr> fx(9,fx1);
    // vector<funPtr> jx{jx1,jx2,jx3};
    // vector<funPtr> gy{f0,f1,f2};
    vector<funPtr> fx{fx1,fx2,fx3};
    vector<funPtr> jx{jx1,jx2,jx3,jx4,jx5,jx6};

    // 观测值，迭代初值，最大迭代次数，tao，精度1，精度2，原方程组，雅可比生成函数
    // LM_Sover lm(ov,iv,10000,1e-3,1e-9,1e-9,fx,jx);
    LM_Sover lm(ov1,iv1,10000,1e-3,1e-9,1e-9,fx,jx);

    lm.Calculation();
    fmt::print("\n======results======\n{}\n", lm.GetResults());

    system("pause");
    return 0;
}