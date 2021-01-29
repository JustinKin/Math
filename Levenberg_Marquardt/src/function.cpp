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
#include <random>

#include "D:\QinJunyou\C\Eigen3\Eigen\Eigen"
#include "D:\QinJunyou\C\Eigen3\Eigen\LU"
#include "D:\FMT\include\fmt\format.h"
#include "D:\FMT\include\fmt\ranges.h"
#include "D:\FMT\include\fmt\os.h"
#include "LM.H"

using namespace std;


vector<pair<double, double>> Test()
{
	float y = 0;
	float x = 0;
	// float a = 0.1;
	// float b = 0.5;
	// float c = 2;
	float a = 3.1415927;
	float b = 2.7182818;
	float c = 0.618034;
	vector<pair<double, double>> values;

	static default_random_engine e(1314);
	static uniform_real_distribution<float> u(-2,2);
	for(int i = 0; i < 9; i++)
	{
		x = u(e);
		y = exp(a * x * x + b * x + c);
		values.emplace_back(make_pair(x,y));
	}
    return values;
}

LM_Sover::LM_Sover(vector<pair<double, double>> ov_, vector<double> iv_, unsigned max_,
                    float tao_, double ac1, double ac2, vector<fxs> fx_) :
                   observed_values(ov_),initial_values(iv_),max_iter(max_),v(2),miu(0),
                   tao(tao_), accuracy_1(ac1),accuracy_2(ac2),results(iv_),fx(fx_)
{
    unsigned rows = ov_.size();
    unsigned cols = iv_.size();
    jacobi.resize(rows,cols);
    gx.resize(rows,1);
    // fmt::print("\nLM_Sover::LM_Sover\n");
}


void LM_Sover::Calculation()
{
    const unsigned dimension = jacobi.cols();
    Eigen::MatrixXd X;  //待求量
    X.resize(dimension,1);
    for(int i = 0; i<dimension; ++i)
    {
        X(i,0) = results[i];
    }

    Jacobi(X);
    // cout << "\nJacobi_1: \n"<<jacobi<<endl;
    Eigen::MatrixXd A;
    A.resize(dimension,dimension);
    A = jacobi.transpose() * jacobi;

    Generate_gx(X);
    Eigen::MatrixXd g;
    g.resize(dimension,1);
    g = jacobi.transpose() * gx;//
    bool found = false;
    auto norm_g = g.rowwise().lpNorm<1>().maxCoeff();
    found = (norm_g <= accuracy_1 ? true : false);

    double max_aii = A(0,0);
    for(int i = 1; i < A.rows(); ++i)
    {
        if(max_aii < A(i,i))
            max_aii = A(i,i);
    }
    miu = tao * max_aii;

    unsigned iter = 0;
    auto h = g;
    auto I = Eigen::MatrixXd::Identity(dimension,dimension);
    while(!found && iter<max_iter)
    {
        ++iter;
        h = (A + miu*I).lu().solve(-g);
        // cout<<"\nh = \n"<<h<<endl;
        // cout<<"\nA = \n"<<A<<endl;
        // cout<<"\nmiu = "<<miu<<endl;
        // cout<<"I = \n"<<I<<endl;
        // cout<<"\ng = \n"<<g<<endl;
        auto norm_h = h.rowwise().lpNorm<1>().maxCoeff();
        auto norm_X  = X.rowwise().lpNorm<1>().maxCoeff();
        if(norm_h <= accuracy_2*(norm_X + accuracy_2))
            found = true;
        else
        {
            auto X_new = X + h;
            auto gx_old = gx;
            auto FX_old = 0.5 * gx_old.squaredNorm();//
            // cout<<"\nX_new = \n"<<X_new<<endl;
            // cout<<"\ngx_old = \n"<<gx_old<<endl;
            // cout<<"\nFX_old = \n"<<FX_old<<endl;

            Generate_gx(X_new);
            auto FX_new = 0.5 * gx.squaredNorm();//
            auto L0_Lh = 0.5 * h.transpose() * ( miu * h - g);//
            auto roh = (FX_old - FX_new) / L0_Lh(0,0);
            // cout<<"\nFX_new = \n"<<FX_new<<endl;
            // cout<<"\nL0_Lh = \n"<<L0_Lh<<endl;
            // cout<<"\nroh = \n"<<roh<<endl;
            if(roh > 0)
            {
                X = X_new;
                Jacobi(X);
                // cout << "\nJacobi_2: \n"<<jacobi<<endl;
                A = jacobi.transpose() * jacobi;
                g = jacobi.transpose() * gx;//
                norm_g = g.rowwise().lpNorm<1>().maxCoeff();
                found = (norm_g <= accuracy_1 ? true : false);
                miu *= std::max(0.3333333, 1-pow((2*roh - 1),3));
                v = 2;
            }
            else
            {
                miu *= v;
                v *= 2;
            }
        }

    }
    for(int i= 0; i<dimension; ++i)
        results[i] = X(i,0);
    cout<<"\n==iterations==\n"<<iter<<endl;


}


void LM_Sover::Jacobi(Eigen::MatrixXd X_)
{
    int dim = jacobi.cols();
    // fmt::print("\nLM_Sover::Jacobi\n");
    vector<double> tmp(dim);
    for(int row =0; row<dim; ++row)
        tmp[row] = X_(row,0);
    for(int row = 0; row <jacobi.rows(); ++row)
    {
        for(int col = 0; col <jacobi.cols(); ++col)
        {
            jacobi(row,col) = (fx[col+1])((observed_values[row]).first,tmp);
        }
    }
    // cout << "\nJacobi_3: \n"<<jacobi<<endl;
}


void LM_Sover::Generate_gx(Eigen::MatrixXd X_)
{
    int dim = jacobi.cols();
    // fmt::print("\nLM_Sover::Generate_gx 1\n");
    vector<double> tmp(dim);
    for(int row =0; row<dim; ++row)
        tmp[row] = X_(row,0);
    // fmt::print("\nLM_Sover::Generate_gx tmp\n{}\n", tmp);
    for(int row = 0; row <gx.rows(); ++row)
    {
        gx(row,0) = (observed_values[row]).second - (fx[0])((observed_values[row]).first,tmp);
    }
    // cout << "\ngx: \n"<<gx<<endl;
}



LM_Sover::~LM_Sover() {}
