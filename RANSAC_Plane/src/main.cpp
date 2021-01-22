//#define EIGEN_USE_MKL_ALL
//#define EIGEN_VECTORIZE_SSE4_2

#include<iostream>
#include<string>
#include <chrono>
#include<Eigen/Eigen>

#include "PLANECONSENSUS.h"

using namespace Eigen;
using namespace std;


int main(int argc, char** argv)
{
    cout << "  Mission Start:  \n\n";
    auto start = chrono::high_resolution_clock::now();
//===============================================================


    // string file = argv[1];
    string file = "SYSU3-10-01_c";
    string filename;
    filename = "D:/QinJunyou/C/Math/RANSAC_Plane/pcd/" + file +".txt";
    PlaneCoefficients plane;
    plane.iter = 10000;
    plane.threshold = 0.006;// calculate inlines
    plane.threshold_dst = 0.01;//check out if distance betwin two pionts is too close
    plane.threshold_iter = 0.85;
    plane.probability = 0.995;
    ComputePlane(filename,plane);
    // OptimizeModel(plane);
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