#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <random>
#include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"
#include "D:\QinJunyou\C\Eigen3\Eigen\LU"
#include "LM.H"

using namespace std;

vector<pair<vector<double>, double>> Test()
{
    double y = 0;
    double x = 0;
    double a = 3.1415927;
    double b = 2.7182818;
    double c = 0.618034;
    vector<pair<vector<double>, double>> values;

    static default_random_engine e(1314);
    static uniform_real_distribution<double> u(-2, 2);
    for (int i = 0; i < 9; i++)
    {
        x = u(e);
        vector<double> tmp;
        tmp.push_back(x);
        y = exp(a * x * x + b * x + c);
        values.emplace_back(make_pair(tmp, y));
    }
    return values;
}

LM_Sover::LM_Sover(vector<pair<vector<double>, double>> ov_, vector<double> iv_, unsigned max_,
                   float tao_, double ac1, double ac2, vector<fxs> fx_, vector<fxs> jx_) :
                   observed_values(ov_), initial_values(iv_), max_iter(max_), v(2), miu(0),
                   tao(tao_), accuracy_1(ac1), accuracy_2(ac2), results(iv_), fx(fx_), jx(jx_)
{
    unsigned rows = ov_.size();
    unsigned cols = iv_.size();
    jacobi.resize(rows, cols);
    gx.resize(rows, 1);
}

void LM_Sover::Calculation()
{
    const unsigned dimension = jacobi.cols();
    Eigen::MatrixXd X; //待求量
    X.resize(dimension, 1);
    for (int i = 0; i < dimension; ++i)
        X(i, 0) = results[i];

    Jacobi(X);
    Eigen::MatrixXd A;
    A.resize(dimension, dimension);
    A = jacobi.transpose() * jacobi;

    Generate_gx(X);
    Eigen::MatrixXd g;
    g.resize(dimension, 1);
    g = jacobi.transpose() * gx;
    bool found = false;
    auto norm_g = g.rowwise().lpNorm<1>().maxCoeff();
    found = (norm_g <= accuracy_1 ? true : false);

    double max_aii = A(0, 0);
    for (int i = 1; i < A.rows(); ++i)
    {
        if (max_aii < A(i, i))
            max_aii = A(i, i);
    }
    miu = tao * max_aii;

    unsigned iter = 0;
    auto h = g;
    auto I = Eigen::MatrixXd::Identity(dimension, dimension);
    while (!found && iter < max_iter)
    {
        ++iter;
        h = (A + miu * I).lu().solve(-g);
        auto norm_h = h.rowwise().lpNorm<1>().maxCoeff();
        auto norm_X = X.rowwise().lpNorm<1>().maxCoeff();
        if (norm_h <= accuracy_2 * (norm_X + accuracy_2))
            found = true;
        else
        {
            auto X_new = X + h;
            auto gx_old = gx;
            auto FX_old = 0.5 * gx_old.squaredNorm();

            Generate_gx(X_new);
            auto FX_new = 0.5 * gx.squaredNorm();
            auto L0_Lh = 0.5 * h.transpose() * (miu * h - g);
            auto roh = (FX_old - FX_new) / L0_Lh(0, 0);
            if (roh > 0)
            {
                X = X_new;
                Jacobi(X);
                A = jacobi.transpose() * jacobi;
                g = jacobi.transpose() * gx; //
                norm_g = g.rowwise().lpNorm<1>().maxCoeff();
                found = (norm_g <= accuracy_1 ? true : false);
                miu *= std::max(0.3333333, 1 - pow((2 * roh - 1), 3));
                v = 2;
            }
            else
            {
                miu *= v;
                v *= 2;
            }
        }
    }
    for (int i = 0; i < dimension; ++i)
        results[i] = X(i, 0);
    cout << "\niterations :\n"
         << iter << endl;
}

void LM_Sover::Jacobi(Eigen::MatrixXd X_)
{
    int dim = jacobi.cols();
    vector<double> tmp(dim);
    for (int row = 0; row < dim; ++row)
        tmp[row] = X_(row, 0);
    int j = dim * jacobi.rows();
    int count = 0;
    for (int row = 0; row < jacobi.rows(); ++row)
    {
        for (int col = 0; col < jacobi.cols(); ++col)
        {
            jacobi(row, col) = (jx[count++])((observed_values[row]).first, tmp);
        }
    }
}

void LM_Sover::Generate_gx(Eigen::MatrixXd X_)
{
    int dim = jacobi.cols();
    vector<double> tmp(dim);
    for (int row = 0; row < dim; ++row)
        tmp[row] = X_(row, 0);
    for (int row = 0; row < gx.rows(); ++row)
    {
        gx(row, 0) = (observed_values[row]).second - (fx[row])((observed_values[row]).first, tmp);
    }
}

LM_Sover::~LM_Sover() {}
