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
#include "LLA_2_ECEF.H"

using namespace std;

int main(int argc, char **argv)
{
    std::ios::sync_with_stdio(false);

/*     LLA_2_ECEF A(113,23,20.3344999999971, 23,4,17.4701999999961, 1.649),
               B(113,23,20.5506000000168, 23,4,18.6941999999981, 1.246),
               C(113,23,22.7175000000278, 23,4,17.6055000000051, 1.149); */
    LLA_2_ECEF A(113,23,20.3405999999958, 23,4,17.4174999999959, 5.402),
               B(113,23,20.5744999999878, 23,4,18.6802000000025, 1.05),
               C(113,23,22.7197000000159, 23,4,17.6434000000008, 0.127);
    double dst_AB = sqrt((A.X - B.X)*(A.X - B.X) + (A.Y - B.Y)*(A.Y - B.Y) + (A.Z - B.Z)*(A.Z - B.Z));
    double dst_AC = sqrt((A.X - C.X)*(A.X - C.X) + (A.Y - C.Y)*(A.Y - C.Y) + (A.Z - C.Z)*(A.Z - C.Z));
    double dst_BC = sqrt((C.X - B.X)*(C.X - B.X) + (C.Y - B.Y)*(C.Y - B.Y) + (C.Z - B.Z)*(C.Z - B.Z));

    fmt::print("A-B distance : {}\n", dst_AB);
    fmt::print("A-C distance : {}\n", dst_AC);
    fmt::print("B-C distance : {}\n", dst_BC);


    system("pause");
    return 0;
}