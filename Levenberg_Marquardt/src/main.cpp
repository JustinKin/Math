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
    auto ov = Test();
	for(const auto &c : ov)
	{
		fmt::print("{: f}, {: f}\n", c.first, c.second);
	}
    vector<float> iv{0,0,0};

    auto fx0 = [](const float x,const vector<float> v) -> float{return exp(v[0] * x*x + v[1] * x + v[2]);};
    auto fx1 = [](const float x,const vector<float> v) -> float{return 2*x * exp(v[0] * x*x + v[1] * x + v[2]);};
    auto fx2 = [](const float x,const vector<float> v) -> float{return x * exp(v[0] * x*x + v[1] * x + v[2]);};
    auto fx3 = [](const float x,const vector<float> v) -> float{return exp(v[0] * x*x + v[1] * x + v[2]);};

    using funPtr = float (*)(float,vector<float>);
    vector<funPtr> gy{fx0,fx1,fx2,fx3};
    LM_Sover lm(ov,iv,100,1,1e-6,1e-6,gy);

    lm.Calculation();
    fmt::print("{: f}, {: f}\n", lm.GetResults());

    system("pause");
    return 0;
}