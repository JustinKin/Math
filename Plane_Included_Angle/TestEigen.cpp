//#define EIGEN_USE_MKL_ALL
//#define EIGEN_VECTORIZE_SSE4_2

#include<iostream>
#include<vector>
#include<string>
#include<cmath>
// #include<math.h>
#include <chrono>
#include<D:/QinJunyou/C/Eigen3/Eigen/Eigen>


using namespace Eigen;
using namespace std;


   int main()
{
    double t1, c=0;
    cout << "  Mission Start:  \n\n";
    auto start = chrono::high_resolution_clock::now();
// =============================================================================

for( ; ;)
{
    // Vector3d a( -129.571, 137.61, 981.975), b( -126.9, 152.4, 980.1) , m(-17.484, 21.1585, 135.622);

    cout<<"Please input 2 plane normals:\n";
    double  a1, b1, c1,
            a2, b2, c2;
    cin >> a1>>b1>>c1
        >> a2>>b2>>c2;
    Vector3d planeA(a1, b1, c1), planeB(a2, b2, c2);
    double angle;
    angle = acos( (abs( planeA.dot(planeB) )) / ((sqrt( planeA.dot(planeA) )) * (sqrt( planeB.dot(planeB) ))) ) ;
    angle = (angle * 180.0) / 3.14159265358979323846;

    cout<<"the 2 planes included angle is :"<<angle<<"\n\n";
}






// =============================================================================
    auto end = chrono::high_resolution_clock::now();
    t1 = std::chrono::duration<double>(end - start).count();
    cout << "Elapsed Time:  \nt1 = " << t1 << " s \n"
         << "**Mission Completed !**\n\n\n"
         << "\a";
    system("pause");
    return 0;
}