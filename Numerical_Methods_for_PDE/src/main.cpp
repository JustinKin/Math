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
#include <string>
#include <vector>
#include "NE4PED.H"

using namespace std;

int main(int argc, char **argv)
{
    unsigned parts = 15;
    string filename = "15parts";
    bool output = false;
    Exercise_271 poisson(2, -1, 1, -1, 1, parts, 0, 1, 0);
    poisson.GeneratePDE();
    poisson.SolveEquation(output);
    poisson.GetResults(filename);

    // unsigned parts = 4;
    // string filename = "4parts_helmholtz";
    // bool output = false;
    // vector<int> k{1, 5, 10, 15, 20};
    // for(int i = 0; i < k.size(); ++i)
    // {
    //   Exercise_272 helmholtz(1, 0, 1, 0, 1, parts, 0, 0, k[i]);
    //   filename += ("_" + to_string(k[i]));
    //   helmholtz.GeneratePDE();
    //   helmholtz.SolveEquation(output);
    //   helmholtz.GetResults(filename);
    // }

    system("pause");
    return 0;
}