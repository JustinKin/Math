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
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <set>
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
    // auto ov = Test();
    vector<std::pair<double, double>> ov{{1,5},{-1,1}};
	for(const auto &c : ov)
	{
		fmt::print("{: f}, {: f}\n", c.first, c.second);
	}
    // vector<double> iv{0,0,0};
    vector<double> iv{1,1};

    // 提供原始程序函数和雅可比矩阵生成函数
    // auto fx0 = [](const double x,const vector<double> v) -> double{return exp(v[0] * x*x + v[1] * x + v[2]);};
    // auto fx1 = [](const double x,const vector<double> v) -> double{return 2*x * exp(v[0] * x*x + v[1] * x + v[2]);};
    // auto fx2 = [](const double x,const vector<double> v) -> double{return x * exp(v[0] * x*x + v[1] * x + v[2]);};
    // auto fx3 = [](const double x,const vector<double> v) -> double{return exp(v[0] * x*x + v[1] * x + v[2]);};

    auto f0 = [](const double x,const vector<double> v) -> double{return v[0] * x + v[1];};
    auto f1 = [](const double x,const vector<double> v) -> double{return x;};
    auto f2 = [](const double x,const vector<double> v) -> double{return 1;};
    using funPtr = double (*)(double,vector<double>);
    // vector<funPtr> gy{fx0,fx1,fx2,fx3};
    vector<funPtr> gy{f0,f1,f2};
    LM_Sover lm(ov,iv,1000,1e-3,1e-9,1e-9,gy);

    lm.Calculation();
    fmt::print("\n======results======\n{}\n", lm.GetResults());

    system("pause");
    return 0;
}