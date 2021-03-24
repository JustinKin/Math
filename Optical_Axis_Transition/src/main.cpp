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
#include "Optical_Axis_Transition.H"

using namespace std;

int main(int argc, char **argv)
{

    // 输入角度单位: 弧度
    // string unit = "rad";
    // double includedAngle = 2.618;
    // double roll = 3.141592654;

    // 输入角度单位: 角度
    string unit = "deg";
    double includedAngle = 150;
    double roll = 180;

    // 初始相机1的方向矢量
    Eigen::Matrix<double, 3, 1> cam1;
    cam1 << 1, 0, 0;

    Optical_Axis_Transition test(unit, includedAngle, cam1);
    test.SolveCam2Vector(roll);
    cout << "\n===Results===\n"
         << test.GetCam2Vector() << "\n\n";

    system("pause");
    return 0;
}