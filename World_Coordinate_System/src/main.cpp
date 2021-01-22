//#define EIGEN_USE_MKL_ALL
//#define EIGEN_VECTORIZE_SSE4_2

#include<iostream>
#include<string>
#include<fstream>
#include<chrono>
#include<Eigen/Eigen>


#include "PLANECONSENSUS.h"
#include "SPHERECONSENSUS.h"

using namespace Eigen;
using namespace std;


int main(int argc, char** argv)
{
    cout << "  Mission Start:  \n\n";
    auto start = chrono::high_resolution_clock::now();
//===============================================================


    // string file = argv[1];
    string file_plane1 = "ground_A_cloud",//ground
           file_plane2 = "board_A_cloud",//borad
           file_sphere = "ball_A_cloud";//ball
    string filename_p1, filename_p2, filename_s;
    filename_p1 = "D:/QinJunyou/C/Math/World_Coordinate_System/pcd/" + file_plane1 +".txt";
    filename_p2 = "D:/QinJunyou/C/Math/World_Coordinate_System/pcd/" + file_plane2 +".txt";
    filename_s  = "D:/QinJunyou/C/Math/World_Coordinate_System/pcd/" + file_sphere +".txt";

    PlaneCoefficients plane1, plane2;
    plane1.iter = 10000;
    plane1.threshold = 0.006;// calculate inlines
    plane1.threshold_dst = 0.01;//check out if distance betwin two pionts is too close
    plane1.threshold_iter = 0.85;
    plane1.probability = 0.995;
    cout<<"=>plane1:\n";
    ComputePlane(filename_p1,plane1);
    plane2.iter = 10000;
    plane2.threshold = 0.006;// calculate inlines
    plane2.threshold_dst = 0.01;//check out if distance betwin two pionts is too close
    plane2.threshold_iter = 0.85;
    plane2.probability = 0.995;
    cout<<"\n=>plane2:\n";
    ComputePlane(filename_p2,plane2);

    SphereCoefficients sphere;
    sphere.iter = 10000;
    sphere.threshold = 0.006;
    sphere.threshold_dst = 0.01;//check out if distance betwin two pionts is too close
    sphere.threshold_iter = 0.85;
    sphere.probability = 0.995;
    cout<<"\n=>sphere:\n";
    ComputeSphere(filename_s,sphere);

    // cout <<"\ntest:\n"<< plane1.coefficients;
    //world coordinate (X,Y,Z) = (n3,n4,n1)
    Vector3d n1(plane1.coefficients[0],plane1.coefficients[1],plane1.coefficients[2]),
             n2(plane2.coefficients[0],plane2.coefficients[1],plane2.coefficients[2]);
    auto n3 = n1.cross(n2);
    auto n4 = n1.cross(n3);
    //calculate Unit orthogonal basis
    Vector3d e1,e2,e3;
    e1 = n3 / sqrt(n3.dot(n3));
    e2 = n4 / sqrt(n4.dot(n4));
    e3 = n1 / sqrt(n1.dot(n1));
    // cout <<"\ne1 =\n"<<e1<<endl;
    // cout <<"\ne2 =\n"<<e2<<endl;
    // cout <<"\ne3 =\n"<<e3<<endl;

    // (cent_x,cent_y,cent_z) is original point of world coordinate (X,Y,Z)
    float cent_x(sphere.coefficients[0]),
          cent_y(sphere.coefficients[1]),
          cent_z(sphere.coefficients[2]);
    //Vector of d
    Vector3d cent(cent_x,cent_y,cent_z);
    // transfer matrix
    Matrix3d T, T_inverse;
    // T << (e1 - cent), (e2 - cent), (e3 - cent);
    T << (e1 ), (e2 ), (e3 );
    cout <<"\nT =\n"<<T<<endl;
    T_inverse = T.inverse();
    cout <<"\nT_inverse =\n"<<T_inverse<<endl;
    cout <<"\nd =\n"<<cent<<endl;
    // write results
    ofstream out("results.txt");
    out <<"=>plane1:\n"<< plane1.coefficients<<endl<<endl;
    out<<"iter : "<< plane1.iter<<"\n"<<
            "pcd :"<< plane1.pcd.size()<<
      "\npcd_in :" << plane1.pcd_in.size()<< endl;

    out <<"\n\n=>plane2:\n"<< plane2.coefficients<<endl<<endl;
    out<<"iter : "<< plane2.iter<<"\n"<<
            "pcd :"<< plane2.pcd.size()<<
      "\npcd_in :" << plane2.pcd_in.size()<< endl;

    out <<"\n\n=>sphere:\n"<< sphere.coefficients<<endl<<endl;
    out<<"iter : "<< sphere.iter<<"\n"<<
            "pcd :"<< sphere.pcd.size()<<
        "\npcd_in :" << sphere.pcd_in.size()<< endl;


    out <<"\nT =\n"<<T<<endl;
    out <<"\nT_inverse =\n"<<T_inverse<<endl;
    out <<"\nd =\n"<<cent<<endl;

    out.close();


    // OptimizeModel(plane);

//===============================================================
    double t1=0;
    auto end = chrono::high_resolution_clock::now();
    t1 = std::chrono::duration<double>(end - start).count();
    cout << "Elapsed Time:  \nt_sum = " << t1 << " s\n\n"
         << "**Mission Completed !**\n\n\n"
         << "\a";
    system("pause");
    return 0;
}