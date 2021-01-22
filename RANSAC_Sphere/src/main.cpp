//#define EIGEN_USE_MKL_ALL
//#define EIGEN_VECTORIZE_SSE4_2

#include<iostream>
#include<string>
#include <chrono>
#include<Eigen/Eigen>

#include "SPHERECONSENSUS.h"

using namespace Eigen;
using namespace std;


int main(int argc, char** argv)
{
    cout << "  Mission Start:  \n\n";
    auto start = chrono::high_resolution_clock::now();
//===============================================================


    string file = argv[1];
    string filename;
    filename = "D:/QinJunyou/C/Math/RANSAC_Sphere/pcd/" + file +".txt";
    SphereCoefficients sphere;
    sphere.iter = 10000;
    sphere.threshold = 0.006;
    sphere.threshold_dst = 0.01;//check out if distance betwin two pionts is too close
    sphere.threshold_iter = 0.85;
    sphere.probability = 0.995;
    ComputeSphere(filename,sphere);
    // OptimizeModel(sphere);
    double t1=0;










//===============================================================
    auto end = chrono::high_resolution_clock::now();
    t1 = std::chrono::duration<double>(end - start).count();
    cout << "Elapsed Time:  \nt_sum = " << t1 << " s\n\n"
         << "**Mission Completed !**\n\n\n"
         << "\a";
    system("pause");
    return 0;
}