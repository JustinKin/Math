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

     /*========================================================*
      *                        测试用例一                        *
      *    3*a*a + b = 7;                                      *
      *    a + 2*b*b = 5;                                      *
      *    6*a*a + 8*b*b = 25.8156;                            *
      *    a = 1.3727454576748315;                             *
      *    b = 1.3467098505500945;                             *
      *========================================================*/

    // 1.提供待求量的迭代初始值
    vector<double> iv1{0,0};

    // 2.观测值类型为 vector<pair<vector<double>, double>>
    // 由观测向量(值)和结果成对(pair)组成的vector
    vector<double> v1{3,1},v2{1,2},v3{6,8};
    vector<pair<vector<double>, double>> ov1{make_pair(v1,7),make_pair(v2,5),make_pair(v3,25.8156)};

    // 3.提供原方程组,传入观测向量(值)和待求量
    auto fx1 = [](const vector<double> x,const vector<double> v) -> double{return x[0] * v[0] * v[0] + x[1] * v[1];};
    auto fx2 = [](const vector<double> x,const vector<double> v) -> double{return x[0] * v[0] + x[1] * v[1] * v[1];};
    auto fx3 = [](const vector<double> x,const vector<double> v) -> double{return x[0] * v[0] * v[0] + x[1] * v[1] * v[1];};

    // 4.提供雅可比矩阵生成函数，注意符号为负,传入观测向量(值)和待求量
    auto jx1 = [](const vector<double> x,const vector<double> v) -> double{return -(6*v[0]);};
    auto jx2 = [](const vector<double> x,const vector<double> v) -> double{return -1;};
    auto jx3 = [](const vector<double> x,const vector<double> v) -> double{return -1;};
    auto jx4 = [](const vector<double> x,const vector<double> v) -> double{return -(4*v[1]);};
    auto jx5 = [](const vector<double> x,const vector<double> v) -> double{return -(12*v[0]);};
    auto jx6 = [](const vector<double> x,const vector<double> v) -> double{return -(16*v[1]);};

    // 5.传入参数为观测向量(值)和待求量的函数指针
    using funPtr = double (*)(vector<double>,vector<double>);
    vector<funPtr> fx_1{fx1,fx2,fx3};
    vector<funPtr> jx_1{jx1,jx2,jx3,jx4,jx5,jx6};

    // 6.观测值，迭代初值，最大迭代次数，tao，精度1，精度2，原方程组，雅可比生成函数
    LM_Sover lm1(ov1,iv1,10000,1e-3,1e-9,1e-9,fx_1,jx_1);
    lm1.Calculation();
    fmt::print("\n===Results===\n{}\n\n\n", lm1.GetResults());


     /*========================================================*
      *                        测试用例二                        *
      *    y = exp(a*x*x + b*x +c);                            *
      *    a = 3.1415927;                                      *
      *    b = 2.7182818;                                      *
      *    c = 0.618034;                                       *
      *    Test()函数生成9组观测值,对迭代初值敏感,有可能不收敛;        *
      *========================================================*/

    vector<double> iv2{0,0,0};

    auto ov2 = Test();
    fmt::print("\n====Observed values====\n");
	for(const auto &c : ov2)
	{
		fmt::print("{: f}, {: f}\n", c.first[0], c.second);
	}

    auto fx01 = [](const vector<double> x,const vector<double> v) -> double{return exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};

    auto jx01 = [](const vector<double> x,const vector<double> v) -> double{return -x[0]*x[0] * exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};
    auto jx02 = [](const vector<double> x,const vector<double> v) -> double{return -x[0] * exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};
    auto jx03 = [](const vector<double> x,const vector<double> v) -> double{return -exp(v[0] * x[0]*x[0] + v[1] * x[0] + v[2]);};

    using funPtr2 = double (*)(vector<double>,vector<double>);
    vector<funPtr2> fx_2(9,fx01);
    vector<funPtr2> jx_2{jx01,jx02,jx03,
                        jx01,jx02,jx03,
                        jx01,jx02,jx03,
                        jx01,jx02,jx03,
                        jx01,jx02,jx03,
                        jx01,jx02,jx03,
                        jx01,jx02,jx03,
                        jx01,jx02,jx03,
                        jx01,jx02,jx03};

    LM_Sover lm2(ov2,iv2,10000,1e-3,1e-9,1e-9,fx_2,jx_2);

    lm2.Calculation();
    fmt::print("\n===Results===\n{}\n", lm2.GetResults());

    system("pause");
    return 0;
}