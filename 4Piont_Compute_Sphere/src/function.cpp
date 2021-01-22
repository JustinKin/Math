#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <random>
#include <math.h>
// #include <algorithm>
#include "SPHERECONSENSUS.h"

using namespace std;
using namespace Eigen;


void ComputeSphere(std::string &filename_,SphereCoefficients &sphere_)
{
    ifstream in;
    in.open(filename_);
    Point p;
    while(!in.eof())
    {
        in >> p.x >> p.y >> p.z;
        sphere_.pcd_in.push_back(p);
    }
    sphere_.pcd_in.erase(sphere_.pcd_in.end()-1);
    in.close();
    for(int i =0; i<1; ++i)
        {
            //compute sphere coefficients
            sphere_.D_in <<
                            sphere_.pcd_in[i].x, sphere_.pcd_in[i].y, sphere_.pcd_in[i].z,
                            sphere_.pcd_in[i+1].x, sphere_.pcd_in[i+1].y, sphere_.pcd_in[i+1].z,
                            sphere_.pcd_in[i+2].x, sphere_.pcd_in[i+2].y, sphere_.pcd_in[i+2].z,
                            sphere_.pcd_in[i+3].x, sphere_.pcd_in[i+3].y, sphere_.pcd_in[i+3].z;
            for(int i = 0,j=1; i<3; ++i,++j)
            {
                Eigen::Vector3f
                    point_i(sphere_.D_in(i,0),sphere_.D_in(i,1),sphere_.D_in(i,2)),
                    point_j(sphere_.D_in(j,0),sphere_.D_in(j,1),sphere_.D_in(j,2));
                sphere_.d(i,0) = 0.5 * (point_j.dot(point_j) - point_i.dot(point_i));
                sphere_.D.row(i) = (point_j - point_i);
            }
            sphere_.Dx = sphere_.D;
            sphere_.Dx.col(0) = sphere_.d;
            sphere_.Dy = sphere_.D;
            sphere_.Dy.col(1) = sphere_.d;
            sphere_.Dz = sphere_.D;
            sphere_.Dz.col(2) = sphere_.d;
            //Cramer's Rule
            float d = (sphere_.D.determinant());
            sphere_.coe[0] = (sphere_.Dx.determinant()) / d;
            sphere_.coe[1] = (sphere_.Dy.determinant()) / d;
            sphere_.coe[2] = (sphere_.Dz.determinant()) / d;
            Eigen::Vector3f
                vcoe((sphere_.coe[0]-sphere_.D_in(0,0)),(sphere_.coe[1]-sphere_.D_in(0,1)),(sphere_.coe[2]-sphere_.D_in(0,2)));
             sphere_.coe[3] = sqrt(vcoe.dot(vcoe));

        }
    sphere_.coe *= 1000;
    sphere_.coe[3] *= 2;
    cout << "cent_x = " <<sphere_.coe[0]<<"\n"
        << "cent_y = " <<sphere_.coe[1]<<"\n"
        << "cent_z = " <<sphere_.coe[2]<<"\n"
        << "radius = " <<sphere_.coe[3]<<"\n";
        //cout << sphere_;


}



