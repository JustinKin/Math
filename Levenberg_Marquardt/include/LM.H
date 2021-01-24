#ifndef __LM_H__
#define __LM_H__

#include <iostream>
#include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"


using funcPtr_generate_jacobi = double (*)(double,std::vector<double>);
using fxs = funcPtr_generate_jacobi;
class LM_Sover
{
public:
    LM_Sover();
    LM_Sover(std::vector<std::pair<double, double>> ov_, std::vector<double> iv_, unsigned max_,
             float tao_, double ac1, double ac2, std::vector<fxs> fx_);
    void Jacobi(Eigen::MatrixXd);
    void Generate_gx(Eigen::MatrixXd);
    // double Norm(Eigen::MatrixXd);
    void Calculation();
    std::vector<double> GetResults() { return results; }
    ~LM_Sover();


private:
    std::vector<std::pair<double, double>> observed_values;
    std::vector<double> initial_values;
    unsigned max_iter;
    unsigned v;
    double miu;
    float tao;
    double accuracy_1;
    double accuracy_2;
    Eigen::MatrixXd jacobi;
    Eigen::MatrixXd gx;     //观测-迭代
    std::vector<double> results;
    std::vector<fxs> fx;    //存放原函数和雅可比生成函数
};

std::vector<std::pair<double, double>> Test();



#endif