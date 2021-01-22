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


void Get4Points(SphereCoefficients &sphere_, int &i_)
{
    int i = i_;
    while(1)
    {

        random_device rde;
        default_random_engine e(rde()+608*i);
        //default_random_engine e(time(0)+608*i);

        uniform_int_distribution<unsigned> rd(0, sphere_.pcd.size() - 1);
        unsigned p1=rd(e), p2=rd(e), p3=rd(e),p4=rd(e);

        Eigen::Matrix4f D_tmp;
        D_tmp <<sphere_.pcd[p1].x, sphere_.pcd[p1].y, sphere_.pcd[p1].z, 1,
                sphere_.pcd[p2].x, sphere_.pcd[p2].y, sphere_.pcd[p2].z, 1,
                sphere_.pcd[p3].x, sphere_.pcd[p3].y, sphere_.pcd[p3].z, 1,
                sphere_.pcd[p4].x, sphere_.pcd[p4].y, sphere_.pcd[p4].z, 1;

        if(abs(D_tmp.determinant()) !=0 && Distance(D_tmp, sphere_))
        {
            sphere_.D_in << sphere_.pcd[p1].x, sphere_.pcd[p1].y, sphere_.pcd[p1].z,
                            sphere_.pcd[p2].x, sphere_.pcd[p2].y, sphere_.pcd[p2].z,
                            sphere_.pcd[p3].x, sphere_.pcd[p3].y, sphere_.pcd[p3].z,
                            sphere_.pcd[p4].x, sphere_.pcd[p4].y, sphere_.pcd[p4].z;
            break;
        }
        ++i;
    }
}


void ComputeSphere(std::string &filename_,SphereCoefficients &sphere_)
{
    ifstream in;
    in.open(filename_);
    Point p;
    while(!in.eof())
    {
        in >> p.x >> p.y >> p.z;
        sphere_.pcd.push_back(p);
    }
    in.close();
    for(int i =0; i<sphere_.iter; ++i)
        {
            Get4Points(sphere_, i);
            //compute sphere coefficients
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
            //compute inliers
            int ins = 0;
            float rad = 0;
            vector <Point> pcd_tmp;
            for(auto c : sphere_.pcd)
            {
                Eigen::Vector3f tmp((sphere_.coe[0]-c.x),(sphere_.coe[1]-c.y),(sphere_.coe[2]-c.z));
                rad = sqrt( tmp.dot(tmp) );
                if(abs(rad - sphere_.coe[3]) <= sphere_.threshold)
                {
                    pcd_tmp.push_back(c);
                    ++ins;
                }
            }
            if(ins > sphere_.inliers)
            {
                sphere_.coefficients = sphere_.coe;
                sphere_.inliers = ins;
                sphere_.pcd_in.resize(pcd_tmp.size());
                swap(pcd_tmp, sphere_.pcd_in);
            }
            double its = 0;
            its = (log(1-sphere_.probability)) / (log(1-(pow((sphere_.inliers/sphere_.pcd.size()),4))));
            //update iter
            if(sphere_.inliers >= (sphere_.threshold_iter*sphere_.pcd.size()))
                {
                    cout<<"iter_i : "<< i<<"\n";
                    break;
                }
            if(its < sphere_.iter && its >0)
                sphere_.iter = its;
        }
        sphere_.coefficients *= 1000;
        sphere_.coefficients[3] *= 2;
        cout << sphere_.coefficients<<endl<<endl;
        cout<<"iter : "<< sphere_.iter<<"\n"<<
              "pcd :"<< sphere_.pcd.size()<<
              "\npcd_in :" << sphere_.pcd_in.size()<< endl;


}

void ShowResult(std::string &filename_,SphereCoefficients &sphere_);

bool Distance(Eigen::Matrix4f &D_,SphereCoefficients &sphere_)
{
    Eigen::Vector3f A, B;
    for(int i = 0 ,j = 1; i<3; ++j)
    {
        A << D_(i,0), D_(i,1), D_(i,2);
        B << D_(j,0), D_(j,1), D_(j,2);
        if(sqrt((A-B).dot(A-B)) < sphere_.threshold_dst)
            return false;
        if(j == 3)
        {
            ++i;
            j = i ;//watch out , ++j in the end , needn't +1
        }
    }
    return true;
}

void OptimizeModel(SphereCoefficients &sphere_)
{
    Eigen::Matrix4f D_tmp;
    auto p = sphere_.pcd_in.begin();
    int amount = ( sphere_.pcd_in.size() - sphere_.pcd_in.size()%4 );
    cout<<"\n\nthe sphere_.pcd_in.size() : "<< sphere_.pcd_in.size() <<"\n";
    cout<<"the amount : "<< amount <<"\n";

    for(int i = 0 ; (i+4) < amount; ++i, ++p )
    {
        D_tmp <<     (*p).x,     (*p).y,     (*p).z, 1,
                 (*(p+1)).x, (*(p+1)).y, (*(p+1)).z, 1,
                 (*(p+2)).x, (*(p+2)).y, (*(p+2)).z, 1,
                 (*(p+3)).x, (*(p+3)).y, (*(p+3)).z, 1;
        if(abs(D_tmp.determinant()) !=0 && Distance(D_tmp, sphere_))
        {
            // cout<<"D_tmp : \n"<< D_tmp <<"\n";
            // for(;;){ }

            for(int k = 0,j=1; k<3; ++k,++j)
            {
                Eigen::Vector3f
                    point_k(D_tmp(k,0),D_tmp(k,1),D_tmp(k,2)),
                    point_j(D_tmp(j,0),D_tmp(j,1),D_tmp(j,2));
                sphere_.d(k,0) = 0.5 * (point_j.dot(point_j) - point_k.dot(point_k));
                sphere_.D.row(k) = (point_j - point_k);
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
                vcoe((sphere_.coe[0]-D_tmp(0,0)),(sphere_.coe[1]-D_tmp(0,1)),(sphere_.coe[2]-D_tmp(0,2)));
            sphere_.coe[3] = sqrt(vcoe.dot(vcoe));
            sphere_.coe_o.push_back(sphere_.coe);
            // sphere_.coe *= 1000;
            // sphere_.coe[3] *= 2;
            // cout<<"sphere_.coe : \n"<< sphere_.coe <<"\n";
            // for(;;){ }

        }
    }
    // accumulate(sphere_.coe_o.begin(),sphere_.coe_o.end(),0);
    Eigen::Vector4f coe_sum;
    for(auto c : sphere_.coe_o)
    {
        coe_sum += c;
    }
    sphere_.coefficients = coe_sum / sphere_.coe_o.size();
    sphere_.coefficients *= 1000;
    sphere_.coefficients[3] *= 2;
    cout<<"\n\nthe optimize sphere : \n"<< sphere_.coefficients <<"\n";
    cout<<"the coe NO : \n"<< sphere_.coe_o.size() <<"\n";
    cout<<"sphere_.coe_o[0] : \n"<< sphere_.coe_o[0] <<"\n";

    //TODO:最小二乘
    //球心基于球的三维正态分布


}